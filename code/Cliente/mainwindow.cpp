    #include "mainwindow.h"
    #include <QStackedWidget>
    #include "telamenu.h"
    #include "telalobby.h"
    #include "telaprocurando.h"
    #include "telapartida.h"
    #include "gameengine.h"
    #include <QDebug>
    #include <QTimer>
    #include <QFile>



    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
        //Cria as telas
        game = GameEngine::getInstance();
        telaMenu = new TelaMenu(this);
        telaMenu->setObjectName("TelaMenu");
        telaLobby = new TelaLobby(this);
        telaLobby->setObjectName("TelaLobby");
        telaProcurando = new TelaProcurando(this);
        telaProcurando->setObjectName("TelaProcurando");
        telaPartida = new TelaPartida(game, this);
        telaPartida->setObjectName("TelaPartida");
        carregarOuGerarUUID();

        // Cria o QStackedWidget e adiciona as telas
        stackedWidget = new QStackedWidget(this);
        stackedWidget->addWidget(telaMenu);
        stackedWidget->addWidget(telaLobby);
        stackedWidget->addWidget(telaProcurando);
        stackedWidget->addWidget(telaPartida);

        setCentralWidget(stackedWidget);

        // Começa pela tela Menu
        stackedWidget->setCurrentWidget(telaMenu);
        previousPage = stackedWidget->currentWidget();

        cliente = new ClienteTruco(this);
        cliente->connectToServer("26.157.51.229", 5000);
        connect(cliente, &ClienteTruco::messageReceived, this, &MainWindow::clientMessageReceived);
        connect(cliente, &ClienteTruco::error, this, &MainWindow::clientError);

        // Conecta os sinais para navegação
        conectarSinais();
    }

    MainWindow::~MainWindow() {
        // Qt cuida da desalocação porque pai é this
    }

    void MainWindow::conectarSinais() {
        // Do Menu para Lobby
        connect(telaMenu, &TelaMenu::iniciarLobby, [this]() {

            QJsonObject command;
            command["type"] = "CRIAR_SALA";
            QJsonObject payload;
            QString userID = carregarOuGerarUUID();
            payload["userID"] = userID;
            command["payload"] = payload;
            cliente->sendCommand(command);

            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaLobby);
            qDebug() << "Mudou para TelaLobby";

        });

        // Do Procurando para abrir a partida (exemplo: aqui você poderia abrir a TelaPartida)
        connect(telaLobby, &TelaLobby::iniciarPartida, [this]() {
            qDebug() << "Quantidade de jogadores na sala: " << game->getJogadores().size();

            QJsonObject command;
            command["type"] = "JOGADORES_CONECTADOS";
            QJsonObject payload;
            QString codigo = game->getCodigoSala();
            payload["codigo"] = codigo;
            command["payload"] = payload;
            cliente->sendCommand(command);

        });

        // Do Procurando para abrir a partida (exemplo: aqui você poderia abrir a TelaPartida)
        connect(telaMenu, &TelaMenu::jogarBot, [this]() {
            game->setModo(1);
            QJsonObject command;
            command["type"] = "CRIAR_SALA";
            QJsonObject payload;
            QString userID = game->getMeuUUID();
            payload["userID"] = userID;
            command["payload"] = payload;
            cliente->sendCommand(command);

        });

        // Sair do Menu fecha app
        connect(telaMenu, &TelaMenu::sair, this, &QMainWindow::close);

        // Do Menu para Procurando
        connect(telaMenu, &TelaMenu::procurarJogo, [this]() {
            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaProcurando);
            QTimer::singleShot(3000, telaProcurando, &TelaProcurando::entrarPartida);
            qDebug() << "Mudou para TelaProcurando";
        });

        // Voltar do Lobby para Menu
        connect(telaLobby, &TelaLobby::voltarMenu, [this]() {

            QJsonObject command;
            command["type"] = "SAIR_SALA";
            QJsonObject payload;
            QString userID = game->getMeuUUID();
            payload["userID"] = userID;
            payload["codigo"] = game->getCodigoSala();
            qDebug()<<game->getCodigoSala();
            command["payload"] = payload;
            cliente->sendCommand(command);

            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaMenu);
            qDebug() << "Voltou para TelaMenu";
        });

        connect(telaPartida, &TelaPartida::sairPartida, [this]() {
            if(previousPage->objectName() == "TelaLobby"){

                previousPage = stackedWidget->currentWidget();
                stackedWidget->setCurrentWidget(telaLobby);
                qDebug() << "Voltou para TelaLobby";

            }else {
                game->setModo(0);
                previousPage = stackedWidget->currentWidget();
                stackedWidget->setCurrentWidget(telaMenu);
                qDebug() << "Voltou para TelaMenu";
            }

        });

        connect(telaMenu, &TelaMenu::entrarSala, [this]() {
            QString codigoDigitado = telaMenu->findChild<QLineEdit*>("codigo")->text();
            game->setCodigoSala(codigoDigitado);
            qDebug()<<game->getCodigoSala();

            QJsonObject command;
            command["type"] = "ENTRAR_SALA";
            QJsonObject payload;
            QString userID = game->getMeuUUID();
            payload["userID"] = userID;
            payload["codigo"] = codigoDigitado;
            command["payload"] = payload;
            cliente->sendCommand(command);

        });

        // Do Procurando para abrir a partida (exemplo: aqui você poderia abrir a TelaPartida)
        connect(telaProcurando, &TelaProcurando::entrarPartida, [this]() {

            qDebug() << "Entrou na partida!";
            QJsonObject command;
            command["type"] = "INICIAR_PARTIDA";
            QJsonObject payload;
            payload["codigo"] = game->getCodigoSala();
            payload["modo"] = 1;
            command["payload"] = payload;
            cliente->sendCommand(command);

            game->iniciarPartida();
            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaPartida);
        });

        // Cancelar busca volta para Menu
        connect(telaProcurando, &TelaProcurando::cancelarBusca, [this]() {
            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaMenu);
            qDebug() << "Busca cancelada, voltou para Lobby";
        });

        connect(telaPartida, &TelaPartida::cartaJogada, [this](int idxCartaJogada) {

            qDebug()<<"carta jogada";

            QJsonObject command;
            command["type"] = "JOGAR_CARTA";
            QJsonObject payload;
            QString userID = game->getMeuUUID();
            payload["userID"] = userID;
            payload["indice"] = game->getMeuIdx();
            payload["codigo"] = game->getCodigoSala();
            payload["indiceCarta"] = idxCartaJogada;
            payload["cartaValor"] = game->getCartasMesa().back().toString().first;
            payload["cartaNaipe"] = game->getCartasMesa().back().toString().second;
            command["payload"] = payload;
            cliente->sendCommand(command);

        });

        connect(this, &MainWindow::atualizarLobby, telaLobby, &TelaLobby::atualizarJogadores);

    }

    void MainWindow::clientMessageReceived(const QJsonObject &message)
    {
        QString type = message["type"].toString();
        QJsonObject payload = message["payload"].toObject();
        qDebug() << (QString("Servidor -> %1: %2").arg(type, QJsonDocument(payload).toJson(QJsonDocument::Compact).constData()));

        if(type == "JOGADORES_CONECTADOS_RESPONSE"){
            int quantidade = payload["message"].toString().split(";").size();
            qDebug() << "Quntidade de jogadores conectados: " << quantidade;

            QJsonObject command;
            command["type"] = "INICIAR_PARTIDA";
            payload["codigo"] = game->getCodigoSala();
            payload["modo"] = quantidade;
            command["payload"] = payload;
            cliente->sendCommand(command);

        }else if (type == "INICIAR_PARTIDA_ATUALIZATION") {


            int modo = payload["modo"].toInt();
            QString valorManilha = payload["valorManilha"].toString();
            QString naipeManilha = payload["naipeManilha"].toString();
            int primeiroJogador = payload["primeiroJogador"].toInt();
            game->setManilha(valorManilha, naipeManilha);
            game->setIdxAtual(primeiroJogador);
            game->setModo(modo);

            for(int i = 0; i < 4 - modo; i++){
                game->addBot(i);
            }

            QString meuUUID = game->getMeuUUID();

            game->distribuirCartas();

            for (int idxJogador = 0; idxJogador < 4; idxJogador++) {
                QJsonObject jogador = payload["Jogador" + QString::number(idxJogador)].toObject();
                for (int idxCarta = 0; idxCarta < 3; idxCarta++) {
                    QString valor = jogador["valor" + QString::number(idxCarta)].toString();
                    QString naipe = jogador["naipe" + QString::number(idxCarta)].toString();
                    game->getJogadores()[idxJogador]->setMao(idxCarta, valor, naipe);
                }
            }

            for(int i = 0; i < 4; i++){
                if(game->getJogadores()[i]->getNome() == meuUUID) game->setMeuIdx(i);
            }

            qDebug()<<"Meu índice: " <<game->getMeuIdx();

            game->iniciarPartida();
            telaPartida->atualizarInterface();
            previousPage = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(telaPartida);

        }else if (type == "ENTRAR_SALA_ATUALIZATION") {

            game->clrJogadores();
            for(auto user : payload["usersSala"].toString().split(";")){
                game->addJogador(user);
            }
            for(auto jogador: game->getJogadores()){
                qDebug() << "Jogador: " << jogador->getNome();
            }

            emit atualizarLobby();

        }else if (type == "JOGAR_CARTA_ATUALIZATION") {

            QString userID = payload["userID"].toString();
            int indice = payload["indiceProx"].toInt();
            int indiceCarta = payload["indiceCarta"].toInt();
            game->setIdxAtual(indice);

            QString cartaNaipe = payload["cartaNaipe"].toString();
            QString cartaValor = payload["cartaValor"].toString();
            int idxJogador = -1;
            for(int i = 0; i < int(game->getJogadores().size()); i++){
                if(game->getJogadores()[i]->getNome() == userID){
                    idxJogador = i;
                }
            }

            if(game->getModo() != 1){
                telaPartida->animarJogada(idxJogador, cartaNaipe, cartaValor, indiceCarta);
                telaPartida->atualizarInterface();
            }

        } else if (type == "ENTRAR_SALA_RESPONSE") {

            if(payload["message"].toString().contains("entrou na sala")){
                previousPage = stackedWidget->currentWidget();
                telaLobby->findChild<QLabel*>("lblCodigoSala")->setText("Código da sala: " + game->getCodigoSala());
                stackedWidget->setCurrentWidget(telaLobby);
            }else{
                mostrarPopup(payload["message"].toString(), 3000);
            }

        } else if (type == "SAIR_SALA_RESPONSE") {

            qDebug() << (payload["message"].toString());
            game->setModo(0);

        } else if (type == "CRIAR_SALA_RESPONSE") {

            game->clrJogadores();
            game->addJogador(game->getMeuUUID());
            game->setCodigoSala(payload["codigo"].toString());
            telaLobby->findChild<QLabel*>("lblCodigoSala")->setText("Código da sala: " + game->getCodigoSala());
            qDebug() << ("Código criado: " + payload["codigo"].toString());

            if(game->getModo() == 1){

                QJsonObject command;
                command["type"] = "INICIAR_PARTIDA";
                payload["codigo"] = game->getCodigoSala();
                payload["modo"] = 1;
                command["payload"] = payload;
                cliente->sendCommand(command);
            }

        } else if (type == "SERVER_STATUS") {

            qDebug() << (QString("Status do Servidor: Hora %1, Uptime %2 segundos.")
                             .arg(payload["serverTime"].toString())
                             .arg(payload["uptimeSeconds"].toInt()));

        } else if (type == "ERROR") {

            qDebug() << ("ERRO do Servidor: " + payload["error"].toString());

        }
    }

    void MainWindow::clientError(const QString &errorString)
    {
        mostrarPopup("Erro do Cliente: " + errorString, 2000);
    }

    QString MainWindow::carregarOuGerarUUID()
    {
        QFile arquivo("id_jogador.txt");

        if (arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&arquivo);
            QString uuid = in.readLine().trimmed();
            if (!uuid.isEmpty()){
                qDebug() << "UUID antes de setMeuUUID:" << uuid;
                game->setMeuUUID(uuid);
                qDebug() << "UUID após setMeuUUID:" << game->getMeuUUID();
                return uuid;
            }
        }

        // Gera novo UUID se não existir
        QString novoUUID = QUuid::createUuid().toString();
        if (arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&arquivo);
            out << novoUUID;
            qDebug() << "UUID antes de setMeuUUID:" << novoUUID;
            game->setMeuUUID(novoUUID);
            qDebug() << "UUID após setMeuUUID:" << game->getMeuUUID();
        }
        return novoUUID;
    }

    void MainWindow::mostrarPopup(QString mensagem, int duracaoMs) {
        QLabel* popup = new QLabel(mensagem, this);
        popup->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 180); color: white; "
                             "padding: 10px; border-radius: 8px; font-weight: bold; }");
        popup->setAlignment(Qt::AlignCenter);

        // Tamanho e posição central da tela
        popup->adjustSize();
        int x = (width() - popup->width()) / 2;
        int y = height() - popup->height() - 50;  // aparece um pouco acima do rodapé
        popup->move(x, y);
        popup->setAttribute(Qt::WA_ShowWithoutActivating); // Não rouba foco
        popup->show();

        // Fecha após a duração
        QTimer::singleShot(duracaoMs, popup, &QLabel::deleteLater);
    }
