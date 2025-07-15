#include "echoserver.h"
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QTimer>

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
        int idxJogadorQueJogou = payload["indice"].toInt();
        int idxCartaQueJogou = payload["indiceCarta"].toInt();
        QString naipe = payload["cartaNaipe"].toString();
        QString valor = payload["cartaValor"].toString();

        game->jogarCarta(idxJogadorQueJogou,idxCartaQueJogou);

        if (clientesPorSala.contains(codigo)) {
            const QList<QTcpSocket*> &clientes = clientesPorSala[codigo];
            qDebug() << "Clientes na sala" << codigo << ": " << clientes.size();
        } else {
            qDebug() << "Sala" << codigo << "não existe na lista clientesPorSala";
        }

        QJsonObject broadcastPayloadInicial;
        broadcastPayloadInicial["codigo"] = codigo;
        broadcastPayloadInicial["userID"] = userID;
        broadcastPayloadInicial["indiceProx"] = (idxJogadorQueJogou + 1) % int(game->getJogadores().size());
        broadcastPayloadInicial["indiceCarta"] = idxCartaQueJogou;
        broadcastPayloadInicial["cartaNaipe"] = naipe;
        broadcastPayloadInicial["cartaValor"] = valor;
        broadcastPayloadInicial["message"] = "Atualização de jogada na sala";

        QJsonObject broadcastInicial;
        broadcastInicial["type"] = type + "_ATUALIZATION";
        broadcastInicial["payload"] = broadcastPayloadInicial;

        broadcastToSala(codigo, broadcastInicial);
        qDebug() << "Turno anterior: "<<game->getTurnoAnterior() << "Turno atual: " << game->getTurno();
        if((game->getTurnoAnterior() + game->getTurno()) % 12 == 0 && game->getTurnoAnterior() != 0){

            game->proximaRodada();
            QJsonObject broadcastPayloadInicial;

            broadcastPayloadInicial["codigo"] = codigo;
            broadcastPayloadInicial["userID"] = game->getJogadores()[game->getIdxJogadorVencedor()]->getNome();
            broadcastPayloadInicial["indiceVencedor"] = game->getIdxJogadorVencedor();
            broadcastPayloadInicial["pntDupla1Rodada1"] = game->getpontosRodadaDupla1()[0];
            broadcastPayloadInicial["pntDupla1Rodada2"] = game->getpontosRodadaDupla1()[1];
            broadcastPayloadInicial["pntDupla1Rodada3"] = game->getpontosRodadaDupla1()[2];
            broadcastPayloadInicial["pntDupla2Rodada1"] = game->getpontosRodadaDupla2()[0];
            broadcastPayloadInicial["pntDupla2Rodada2"] = game->getpontosRodadaDupla2()[1];
            broadcastPayloadInicial["pntDupla2Rodada3"] = game->getpontosRodadaDupla2()[2];
            broadcastPayloadInicial["message"] = "Atualização de vencedor na sala";

            QJsonObject broadcastInicial;
            broadcastInicial["type"] = "VENCEDOR_RODADA_ATUALIZATION";
            broadcastInicial["payload"] = broadcastPayloadInicial;

            broadcastToSala(codigo, broadcastInicial);
        }



        int proximoJogadorIdx = (idxJogadorQueJogou + 1) % game->getJogadores().size();

        if (game->getJogadores()[proximoJogadorIdx]->getNome().contains("Bot")) {
            qDebug() << "Próximo jogador é um bot. Agendando primeira jogada do bot.";
            QTimer::singleShot(1000, this, [this, codigo, proximoJogadorIdx]() {
                processarProximoTurnoBot(codigo, proximoJogadorIdx);
            });
        } else {
            qDebug() << "Próximo jogador não é um bot. Fim da sequência de jogadas.";
        }
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

void EchoServer::processarProximoTurnoBot(const QString& codigoSala, int idxBotAtual) {
    qDebug() << "Processando turno do bot no índice:" << idxBotAtual << "para a sala:" << codigoSala;

    if (idxBotAtual >= int(game->getJogadores().size()) ||
        !game->getJogadores()[idxBotAtual]->getNome().contains("Bot")) {
        qDebug() << "Sequência de bots interrompida: jogador no índice " << idxBotAtual << " não é bot ou índice inválido.";
        return;
    }

    Jogador* bot = game->getJogadores()[idxBotAtual];

    int qtdMaoBot = bot->getMao().size();
    if (qtdMaoBot == 0) {
        qDebug() << "Bot" << bot->getNome() << "não tem mais cartas na mão. Parando sequência de jogadas do bot.";
        return;
    }

    int cartaAleatoriaIdx = QRandomGenerator::global()->bounded(0, static_cast<int>(qtdMaoBot));
    Carta cartaQueOBoVaiJogar = bot->getMao()[cartaAleatoriaIdx];

    game->jogarCarta(idxBotAtual, cartaAleatoriaIdx);

    QJsonObject broadcastPayloadBot;
    broadcastPayloadBot["codigo"] = codigoSala;
    broadcastPayloadBot["userID"] = bot->getNome();
    broadcastPayloadBot["indiceProx"] = (idxBotAtual + 1) % int(game->getJogadores().size());
    broadcastPayloadBot["indiceCarta"] = cartaAleatoriaIdx;
    broadcastPayloadBot["cartaNaipe"] = cartaQueOBoVaiJogar.toString().second;
    broadcastPayloadBot["cartaValor"] = cartaQueOBoVaiJogar.toString().first;
    broadcastPayloadBot["message"] = "Atualização de jogada do bot na sala";

    QJsonObject broadcastBot;
    broadcastBot["type"] = "JOGAR_CARTA_ATUALIZATION";
    broadcastBot["payload"] = broadcastPayloadBot;

    broadcastToSala(codigoSala, broadcastBot);

    qDebug() << "Turno anterior: "<<game->getTurnoAnterior() << "Turno atual: " << game->getTurno();
    if((game->getTurnoAnterior() + game->getTurno()) % 12 == 0 && game->getTurnoAnterior() != 0){

        game->proximaRodada();
        QJsonObject broadcastPayloadInicial;

        broadcastPayloadInicial["codigo"] = codigoSala;
        broadcastPayloadInicial["userID"] = game->getJogadores()[game->getIdxJogadorVencedor()]->getNome();
        broadcastPayloadInicial["indiceVencedor"] = game->getIdxJogadorVencedor();
        broadcastPayloadInicial["pntDupla1Rodada1"] = game->getpontosRodadaDupla1()[0];
        broadcastPayloadInicial["pntDupla1Rodada2"] = game->getpontosRodadaDupla1()[1];
        broadcastPayloadInicial["pntDupla1Rodada3"] = game->getpontosRodadaDupla1()[2];
        broadcastPayloadInicial["pntDupla2Rodada1"] = game->getpontosRodadaDupla2()[0];
        broadcastPayloadInicial["pntDupla2Rodada2"] = game->getpontosRodadaDupla2()[1];
        broadcastPayloadInicial["pntDupla2Rodada3"] = game->getpontosRodadaDupla2()[2];
        broadcastPayloadInicial["message"] = "Atualização de vencedor na sala";

        QJsonObject broadcastInicial;
        broadcastInicial["type"] = "VENCEDOR_RODADA_ATUALIZATION";
        broadcastInicial["payload"] = broadcastPayloadInicial;

        broadcastToSala(codigoSala, broadcastInicial);
    }

    int proximoJogadorNaSequencia = (idxBotAtual + 1) % game->getJogadores().size();

    if (game->getJogadores()[proximoJogadorNaSequencia]->getNome().contains("Bot")) {
        qDebug() << "Próximo jogador também é um bot. Agendando próxima jogada do bot.";
        QTimer::singleShot(1000, this, [this, codigoSala, proximoJogadorNaSequencia]() {
            processarProximoTurnoBot(codigoSala, proximoJogadorNaSequencia);
        });
    } else {
        qDebug() << "Próximo jogador não é um bot. Fim da sequência de jogadas de bots.";
    }
}

