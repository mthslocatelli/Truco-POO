#include "clientetruco.h"

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
    // Adiciona os novos dados ao buffer existente
    m_buffer.append(m_tcpSocket->readAll());

    QDataStream in(&m_buffer, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15); // Deve corresponder à versão do servidor

    while (true) {
        if (m_nextBlockSize == 0) { // Ainda não lemos o tamanho da próxima mensagem
            if (m_buffer.size() < (int)sizeof(quint32)) {
                break; // Não há bytes suficientes para o tamanho, espere mais dados
            }
            in >> m_nextBlockSize; // Lê o tamanho da próxima mensagem
            qDebug() << "Esperando mensagem de" << m_nextBlockSize << "bytes.";
        }

        // Verifica se temos dados suficientes para a mensagem completa (tamanho + dados)
        if (m_buffer.size() < m_nextBlockSize + (int)sizeof(quint32)) {
            break; // Não há dados suficientes, espere mais dados
        }

        // Se chegamos aqui, temos a mensagem completa
        QByteArray jsonData;
        // Avança o stream para o início dos dados JSON
        jsonData = m_buffer.mid(sizeof(quint32), m_nextBlockSize);

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "Mensagem JSON inválida recebida:" << jsonData;
            // Erro: descarta esta mensagem e tenta ler a próxima
            m_nextBlockSize = 0;
            m_buffer.remove(0, (int)sizeof(quint32) + jsonData.size());
            continue;
        }

        emit messageReceived(doc.object());

        // Remove a mensagem processada do buffer
        m_buffer.remove(0, (int)sizeof(quint32) + m_nextBlockSize);
        m_nextBlockSize = 0; // Reseta para a próxima mensagem
    }
}

void ClienteTruco::onError(QAbstractSocket::SocketError socketError)
{
    QString errString = m_tcpSocket->errorString();
    qWarning() << "Erro do Cliente TCP (" << socketError << "):" << errString;
    emit error(errString);
}
