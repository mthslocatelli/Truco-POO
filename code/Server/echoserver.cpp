#include "echoserver.h"

EchoServer::EchoServer(quint16 port, QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer(this);

    connect(m_tcpServer, &QTcpServer::newConnection, this, &EchoServer::onNewConnection);

    if (m_tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "EchoServer escutando na porta" << port;
    } else {
        qDebug() << "Erro ao iniciar EchoServer:" << m_tcpServer->errorString();
    }
}

EchoServer::~EchoServer()
{
    m_tcpServer->close();
    // Os QTcpSocket serão deletados automaticamente quando o m_tcpServer for destruído ou por deleteLater()
}

void EchoServer::onNewConnection()
{
    QTcpSocket *pSocket = m_tcpServer->nextPendingConnection();
    qDebug() << "Nova conexão de cliente:" << pSocket->peerAddress().toString() << ":" << pSocket->peerPort();

    connect(pSocket, &QTcpSocket::readyRead, this, &EchoServer::onReadyRead);
    connect(pSocket, &QTcpSocket::disconnected, this, &EchoServer::onDisconnected);
    connect(pSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &EchoServer::onError);

    // Inicializa o tamanho do próximo bloco e buffer para o novo cliente
    m_clientNextBlockSize[pSocket] = 0;
    m_clientBuffers[pSocket] = QByteArray();
}

void EchoServer::onReadyRead()
{
    QTcpSocket *pClient = qobject_cast<QTcpSocket *>(sender());
    if (!pClient) return;

    QByteArray &buffer = m_clientBuffers[pClient];
    quint32 &nextBlockSize = m_clientNextBlockSize[pClient];

    buffer.append(pClient->readAll());

    QDataStream in(&buffer, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_15); // Use a versão do Qt que você está usando (ou Qt_6_0 para Qt6)

    while (true) {
        if (nextBlockSize == 0) { // Ainda não lemos o tamanho da próxima mensagem
            if (buffer.size() < (int)sizeof(quint32)) {
                break; // Não há bytes suficientes para o tamanho, espere mais dados
            }
            in >> nextBlockSize; // Lê o tamanho da próxima mensagem
            qDebug() << "Esperando mensagem de" << nextBlockSize << "bytes.";
        }

        // Verifica se temos dados suficientes para a mensagem completa (tamanho + dados)
        if (buffer.size() < nextBlockSize + (int)sizeof(quint32)) {
            break; // Não há dados suficientes, espere mais dados
        }

        // Se chegamos aqui, temos a mensagem completa
        QByteArray jsonData;
        // Avança o stream para o início dos dados JSON
        jsonData = buffer.mid(sizeof(quint32), nextBlockSize);

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "Mensagem JSON inválida recebida:" << jsonData;
            // Erro: descarta esta mensagem e tenta ler a próxima
            nextBlockSize = 0;
            buffer.remove(0, (int)sizeof(quint32) + jsonData.size());
            continue;
        }

        QJsonObject obj = doc.object();
        qDebug() << "Comando recebido:" << obj["type"].toString();

        processCommand(pClient, obj);

        // Remove a mensagem processada do buffer
        buffer.remove(0, (int)sizeof(quint32) + nextBlockSize);
        nextBlockSize = 0; // Reseta para a próxima mensagem
    }
}

void EchoServer::onDisconnected()
{
    QTcpSocket *pClient = qobject_cast<QTcpSocket *>(sender());
    if (pClient) {
        qDebug() << "Cliente desconectado:" << pClient->peerAddress().toString();
        m_clientBuffers.remove(pClient); // Limpa o buffer
        m_clientNextBlockSize.remove(pClient); // Limpa o tamanho do bloco
        pClient->deleteLater(); // Agenda a exclusão do socket
    }
}

void EchoServer::onError(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *pClient = qobject_cast<QTcpSocket *>(sender());
    if (pClient) {
        qWarning() << "Erro no socket do cliente" << pClient->peerAddress().toString() << ":" << pClient->errorString() << "(Error code:" << socketError << ")";
    }
}

void EchoServer::sendResponse(QTcpSocket *client, const QJsonObject &message)
{
    QJsonDocument doc(message);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);

    out << (quint32)jsonData.size(); // Primeiro, o tamanho da mensagem JSON
    block.append(jsonData);          // Depois, a própria mensagem JSON

    client->write(block);
    qDebug() << "Resposta enviada:" << doc.toJson(QJsonDocument::Compact);
}

void EchoServer::processCommand(QTcpSocket *client, const QJsonObject &command)
{
    QString type = command["type"].toString();
    QJsonObject payload = command["payload"].toObject();
    QJsonObject responsePayload;
    QJsonObject response;

    if (type == "CRIAR_SALA") {
        game.gerarCodigoSala();
        responsePayload["codigo"] = game.getCodigoSala();
        response["type"] = "CODIGO_CRIAR_SALA";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    } else if (type == "HELLO") {
        responsePayload["message"] = "Olá, cliente!";
        response["type"] = "WELCOME";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    } else if (type == "PING") {
        responsePayload["timestamp"] = QDateTime::currentMSecsSinceEpoch();
        response["type"] = "PONG";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    } else if (type == "ECHO") {
        responsePayload["codigo_sala"] = payload["codigo_sala"].toString();
        responsePayload["userID"] = payload["userID"].toString();
        response["type"] = type+"_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    } else if (type == "ENTRAR_SALA") {
        responsePayload["serverTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        responsePayload["uptimeSeconds"] = (QDateTime::currentMSecsSinceEpoch() - QCoreApplication::startingUp()) / 1000;
        response["type"] = "ENTRAR_SALA";
        response["payload"] = payload["codigo_da_sala"].toString();
        sendResponse(client, response);
    } else {
        responsePayload["error"] = "Comando desconhecido: " + type;
        response["type"] = "ERROR";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    }
}
