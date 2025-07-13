#include "clientetruco.h"
#include <QTimer>

ClienteTruco::ClienteTruco(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
    m_nextBlockSize = 0; // Inicializa o tamanho do próximo bloco

    connect(m_tcpSocket, &QTcpSocket::connected, this, &ClienteTruco::onConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ClienteTruco::onDisconnected);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ClienteTruco::onReadyRead);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &ClienteTruco::onError);
}

void ClienteTruco::connectToServer(const QString &address, quint16 port)
{
    qDebug() << "Tentando conectar a" << address << ":" << port;
    m_tcpSocket->connectToHost(address, port);
}

void ClienteTruco::disconnectFromServer()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_tcpSocket->disconnectFromHost();
    }
}

void ClienteTruco::sendCommand(const QJsonObject &command)
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState) {
        QJsonDocument doc(command);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15); // Deve corresponder à versão do servidor

        out << (quint32)jsonData.size(); // Primeiro, escreve o tamanho do JSON
        block.append(jsonData);          // Depois, o próprio JSON

        qDebug()<<(quint32)jsonData.size();
        m_tcpSocket->write(block);
        qDebug() << "Comando enviado:" << doc.toJson(QJsonDocument::Compact);
    } else {
        qWarning() << "Não conectado ao servidor. Comando não enviado.";
        emit error("Não conectado ao servidor.");
    }
}

void ClienteTruco::onConnected()
{
    qDebug() << "Cliente TCP conectado!";
    emit connected();
}

void ClienteTruco::onDisconnected()
{
    qDebug() << "Cliente TCP desconectado!";
    m_buffer.clear(); // Limpa o buffer ao desconectar
    m_nextBlockSize = 0; // Reseta o tamanho do bloco
    emit disconnected();
}

void ClienteTruco::onReadyRead()
{
    m_buffer.append(m_tcpSocket->readAll());

    while (true) {
        // Precisamos de pelo menos 4 bytes para o tamanho
        if (m_nextBlockSize == 0) {
            if (m_buffer.size() < (int)sizeof(quint32))
                break;

            // Lê os 4 primeiros bytes (tamanho do bloco)
            QDataStream sizeStream(m_buffer.left(sizeof(quint32)));
            sizeStream.setVersion(QDataStream::Qt_5_15);
            sizeStream >> m_nextBlockSize;

            qDebug() << "Esperando mensagem de" << m_nextBlockSize << "bytes.";
        }

        // Verifica se já temos a mensagem completa
        if (m_buffer.size() < (int)(sizeof(quint32) + m_nextBlockSize))
            break;

        // Extrai o JSON
        QByteArray jsonData = m_buffer.mid(sizeof(quint32), m_nextBlockSize);
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        if (!doc.isNull() && doc.isObject()) {
            emit messageReceived(doc.object());
        } else {
            qWarning() << "Mensagem JSON inválida recebida:" << jsonData;
        }

        // Remove do buffer a mensagem processada (tamanho + dados)
        m_buffer.remove(0, sizeof(quint32) + m_nextBlockSize);
        m_nextBlockSize = 0;
    }
}

void ClienteTruco::onError(QAbstractSocket::SocketError socketError)
{
    QString errString = m_tcpSocket->errorString();
    qWarning() << "Erro do Cliente TCP (" << socketError << "):" << errString;
    emit error(errString);
    QTimer::singleShot(3000, this, [this]() {
        m_tcpSocket->connectToHost("26.157.51.229", 5000);
    });

}
