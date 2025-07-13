#include "echoserver.h"
#include <QSqlQuery>

EchoServer::EchoServer(GameEngine* gameEngine, quint16 port, QObject *parent) : QObject(parent), game(gameEngine)
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

    out << (quint32)jsonData.size();  // escreve o tamanho
    out.writeRawData(jsonData.constData(), jsonData.size());  // escreve os dados JSON

    client->write(block);
    qDebug() << "Resposta enviada:" << doc.toJson(QJsonDocument::Compact);
}

void EchoServer::processCommand(QTcpSocket *client, const QJsonObject &command)
{
    QString type = command["type"].toString();
    QJsonObject payload = command["payload"].toObject();
    QJsonObject responsePayload;
    QJsonObject response;

    if (type == "JOGADORES_CONECTADOS") {

        QString codigo = payload["codigo"].toString();
        QString conectados;
        if (clientesPorSala.contains(codigo)) {
            const QList<QTcpSocket*> &clientes = clientesPorSala[codigo];
            if(clientes.size() > 1){
                for(auto cliente : clientes){
                    conectados += cliente->peerAddress().toString() + ";";
                }
                conectados.chop(1);
            }else{
                conectados += clientes.first()->peerAddress().toString();
            }
        }
        responsePayload["message"] = conectados;
        response["type"] = type+"_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);

    }else if (type == "CRIAR_SALA") {


        game->gerarCodigoSala(payload["userID"].toString());
        QString codigo = game->getCodigoSala();
        QString userID = payload["userID"].toString();

        if (!clientesPorSala.contains(codigo)) {
            clientesPorSala[codigo] = QList<QTcpSocket*>();
        }

        if (!clientesPorSala[codigo].contains(client)) {
            clientesPorSala[codigo].append(client);
        }

        if (clientesPorSala.contains(codigo)) {
            const QList<QTcpSocket*> &clientes = clientesPorSala[codigo];
            qDebug() << "Clientes na sala" << codigo << ": " << clientes.size();
        } else {
            qDebug() << "Sala" << codigo << "não existe na lista clientesPorSala";
        }

        responsePayload["codigo"] = codigo;
        response["type"] = "CRIAR_SALA_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);

    } else if (type == "ENTRAR_SALA") {
        QString codigo = payload["codigo"].toString();
        QString userID = payload["userID"].toString();
        QString result = game->entrarSala(codigo, userID);

        responsePayload["message"] = result;
        response["type"] = "ENTRAR_SALA_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);

        if (result.contains("entrou na sala")) {
            if (!clientesPorSala.contains(codigo)) {
                clientesPorSala[codigo] = QList<QTcpSocket*>();
            }

            if (!clientesPorSala[codigo].contains(client)) {
                clientesPorSala[codigo].append(client);
            }

            if (clientesPorSala.contains(codigo)) {
                const QList<QTcpSocket*> &clientes = clientesPorSala[codigo];
                qDebug() << "Clientes na sala" << codigo << ": " << clientes.size();
            } else {
                qDebug() << "Sala" << codigo << "não existe na lista clientesPorSala";
            }



            // --- Aqui pega os userIDs da sala do banco e concatena
            QStringList userIDsList;
            {
                const QString connectionName = "sala_connection";
                QSqlDatabase db;
                if (QSqlDatabase::contains(connectionName)) {
                    db = QSqlDatabase::database(connectionName);
                } else {
                    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
                    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/salas.db");
                }

                if (db.open()) {
                    QSqlQuery query(db);
                    query.prepare("SELECT userID0, userID1, userID2, userID3 FROM salas WHERE codigo = :codigo");
                    query.bindValue(":codigo", codigo);
                    if (query.exec() && query.next()) {
                        for (int i = 0; i < 4; ++i) {
                            QString id = query.value(i).toString();
                            if (!id.isEmpty())
                                userIDsList.append(id);
                        }
                    }
                    db.close();
                }
            }

            // Monta string separada por ";"
            QString userIDsConcat = userIDsList.join(";");

            QStringList listaClientes;
            for (QTcpSocket* socket : clientesPorSala[codigo]) {
                listaClientes << socket->peerAddress().toString();  // ou algum outro identificador
            }

            // --- Monta a mensagem para broadcast para TODOS os da sala
            QJsonObject broadcastPayload;
            broadcastPayload["codigo"] = codigo;
            broadcastPayload["usersSala"] = userIDsConcat;
            broadcastPayload["message"] = "Atualização dos jogadores na sala";

            QJsonObject broadcast;
            broadcast["type"] = "ENTRAR_SALA_ATUALIZATION";
            broadcast["payload"] = broadcastPayload;

            // Envia para todos da sala
            broadcastToSala(codigo, broadcast);
        }


    } else if (type == "SAIR_SALA") {
        QString userID = payload["userID"].toString();
        QString codigo = payload["codigo"].toString();
        game->sairSala(payload["userID"].toString());
        responsePayload["message"] = "Jogador "+payload["userID"].toString()+" saiu da sala!";
        response["type"] = "SAIR_SALA_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);

        if (clientesPorSala.contains(codigo)) {
            clientesPorSala[codigo].removeAll(client); // remove o cliente
        }

    } else if (type == "JOGAR_CARTA") {

        QString codigo = payload["codigo"].toString();
        QString userID = payload["userID"].toString();
        int idx = payload["indice"].toInt();
        QString naipe = payload["cartaNaipe"].toString();
        QString valor = payload["cartaValor"].toString();

        if (clientesPorSala.contains(codigo)) {
            const QList<QTcpSocket*> &clientes = clientesPorSala[codigo];
            qDebug() << "Clientes na sala" << codigo << ": " << clientes.size();
        } else {
            qDebug() << "Sala" << codigo << "não existe na lista clientesPorSala";
        }

        QJsonObject broadcastPayload;
        broadcastPayload["codigo"] = codigo;
        broadcastPayload["userID"] = userID;
        broadcastPayload["indiceProx"] = idx + 1;
        broadcastPayload["indiceCarta"] = payload["indiceCarta"].toInt();
        broadcastPayload["cartaNaipe"] = naipe;
        broadcastPayload["cartaValor"] = valor;
        broadcastPayload["message"] = "Atualização de jogada na sala";

        QJsonObject broadcast;
        broadcast["type"] = type+"_ATUALIZATION";
        broadcast["payload"] = broadcastPayload;

        // Envia para todos da sala
        broadcastToSala(codigo, broadcast);

    } else if (type == "INICIAR_PARTIDA") {

        QString codigo = payload["codigo"].toString();
        int modo = payload["modo"].toInt();
        game->setModo(modo);

        for(int i = 0; i < 4 - modo; i++){
            game->addBot(i);
        }
        game->distribuirCartas();
        game->definirManilha();

        qDebug() << "Quantidade de jogadores: " << game->getJogadores().size();
        qDebug() << "modo: " << game->getModo();

        game->iniciarPartida();

        QJsonObject broadcastPayload;
        QJsonObject broadcastPayloadMao;
        int idxJogador = 0;
        for(int i = 0; i < 4; i++){
            idxJogador = i;
            qDebug() << game->getJogadores()[i]->getNome();
            for(int i = 0; i < 3; i++){
                broadcastPayloadMao["valor" + QString::number(i)] = game->getJogadores()[idxJogador]->getMao()[i].toString().first;
                broadcastPayloadMao["naipe" + QString::number(i)] = game->getJogadores()[idxJogador]->getMao()[i].toString().second;
            }
            broadcastPayload["Jogador" + QString::number(i)] = broadcastPayloadMao;
        }
        broadcastPayload["message"] = "Carregando cartas dos jogadores";
        broadcastPayload["modo"] = modo;
        broadcastPayload["primeiroJogador"] = 0;
        broadcastPayload["valorManilha"] = game->getManilha().toString().first;
        broadcastPayload["naipeManilha"] = game->getManilha().toString().second;

        QJsonObject broadcast;
        broadcast["type"] = "INICIAR_PARTIDA_ATUALIZATION";
        broadcast["payload"] = broadcastPayload;

        // Envia para todos da sala
        broadcastToSala(codigo, broadcast);

    }else if (type == "ECHO") {

        responsePayload["codigo_sala"] = payload["codigo_sala"].toString();
        responsePayload["userID"] = payload["userID"].toString();
        response["type"] = type+"_RESPONSE";
        response["payload"] = responsePayload;
        sendResponse(client, response);

    } else {

        responsePayload["error"] = "Comando desconhecido: " + type;
        response["type"] = "ERROR";
        response["payload"] = responsePayload;
        sendResponse(client, response);
    }
}

void EchoServer::broadcastToSala(const QString& codigoSala, const QJsonObject& message) {
    if (!clientesPorSala.contains(codigoSala)) return;
    for (QTcpSocket* cliente : clientesPorSala[codigoSala]) {
        if (cliente->state() == QAbstractSocket::ConnectedState) {
            qDebug() << "Broadcast para" << cliente->peerAddress().toString() << ":" << cliente->peerPort();
            sendResponse(cliente, message);
        }
    }
}


