#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->showFullScreen();

    cliente = new ClienteTruco(this);
    cliente->connectToServer("192.168.0.162", 5000);

    // connect(cliente, &ClienteTruco::connected, this, &MainWindow::clientConnected);
    // connect(cliente, &ClienteTruco::disconnected, this, &MainWindow::clientDisconnected);
    connect(cliente, &ClienteTruco::messageReceived, this, &MainWindow::clientMessageReceived);
    connect(cliente, &ClienteTruco::error, this, &MainWindow::clientError);

    atualizarInterface();
    cartasBot1 = { ui->cartaJogador1_1, ui->cartaJogador1_2, ui->cartaJogador1_3 };
    cartasBot2 = { ui->cartaJogador2_1, ui->cartaJogador2_2, ui->cartaJogador2_3 };
    cartasBot3 = { ui->cartaJogador3_1, ui->cartaJogador3_2, ui->cartaJogador3_3 };
    // Define e posiciona a imagem de fundo da mesa
    QPixmap mesa(":/imagens/mesa.png");
    ui->mesa->setPixmap(mesa);
    ui->mesa->setGeometry(QRect(200, 100, this->width() - 400, this->height() - 200));

    QPixmap cartaAdversario(":/imagens/costas_carta.png");

    // Tamanho padrão das cartas
    const int largura = 80 * 1.6;
    const int altura = 80;
    const int espacamento1 = 10;

    // --- Jogador 1 (esquerda)
    QLabel* cartasJ1[3] = { ui->cartaJogador1_1, ui->cartaJogador1_2, ui->cartaJogador1_3 };
    int xJ1 = ui->mesa->x() + 20;
    int yBaseJ1 = 480;

    for (int i = 0; i < 3; ++i) {
        cartasJ1[i]->setGeometry(xJ1, yBaseJ1 - i * (altura + espacamento1), largura, altura);
        cartasJ1[i]->setPixmap(cartaAdversario);
        cartasJ1[i]->setScaledContents(true);
    }

    // --- Jogador 3 (direita)
    QLabel* cartasJ3[3] = { ui->cartaJogador3_1, ui->cartaJogador3_2, ui->cartaJogador3_3 };
    int xJ3 = ui->mesa->x() + ui->mesa->width() - largura - 20;
    int yBaseJ3 = 480;

    for (int i = 0; i < 3; ++i) {
        cartasJ3[i]->setGeometry(xJ3, yBaseJ3 - i * (altura + espacamento1), largura, altura);
        cartasJ3[i]->setPixmap(cartaAdversario);
        cartasJ3[i]->setScaledContents(true);
    }

    // --- Jogador 2 (superior)
    QLabel* cartasJ2[3] = { ui->cartaJogador2_1, ui->cartaJogador2_2, ui->cartaJogador2_3 };
    int larguraJ2 = 80;
    int alturaJ2 = 80 * 1.6;
    int xBaseJ2 = (this->width() - (3 * larguraJ2 + 2 * espacamento1)) / 2;
    int yJ2 = ui->mesa->y() + 10;

    for (int i = 0; i < 3; ++i) {
        cartasJ2[i]->setGeometry(xBaseJ2 + i * (larguraJ2 + espacamento1), yJ2, larguraJ2, alturaJ2);
        cartasJ2[i]->setPixmap(cartaAdversario);
        cartasJ2[i]->setScaledContents(true);
    }

    QLabel* circuloLabel[3] = { ui->pontoNos1, ui->pontoNos2, ui->pontoNos3 };
    QLabel* circuloLabel1[3] = { ui->pontoEles1, ui->pontoEles2, ui->pontoEles3 };
    for (int i = 0; i < 3; ++i) {
        circuloLabel[i]->setScaledContents(true);
        circuloLabel1[i]->setScaledContents(true);
        circuloLabel[i]->move(ui->mesa->x() + i * 22, 70); // espaçamento de 22px
        circuloLabel1[i]->move(ui->mesa->x() + ui->mesa->width() - i * 22, 70); // espaçamento de 22px
    }

    QPushButton* botoes[3] = { ui->btnCarta1, ui->btnCarta2, ui->btnCarta3 };
    int larguraCarta = 120;
    int espacamento = 20;
    int xInicial = 560;
    int yInicial = 570;

    for (int i = 0; i < 3; ++i) {
        botoes[i]->setText("");
        botoes[i]->setStyleSheet(
            "QPushButton {"
            "  border-radius: 5px;"
            "  border: 2px solid white;"
            "  background-color: white;"
            "}"
            );
        botoes[i]->move(xInicial + i * (larguraCarta + espacamento), yInicial);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::atualizarInterface() {
    auto mao = game.getJogadores()[0]->getMao();
    this->setStatusBar(nullptr);

    // Configuração dos botões das cartas
    QPushButton* botoes[3] = { ui->btnCarta1, ui->btnCarta2, ui->btnCarta3 };
    for (int i = 0; i < 3; ++i) {
        QString naipe = mao[i].toString().second;
        QString valor = mao[i].toString().first;

        QIcon icon(":/imagens/cartas_" + naipe + "/" + naipe + "_" + valor + ".jpg");
        if (icon.isNull())
            icon.addFile(":/imagens/cartas_" + naipe + "/" + naipe + "_" + valor + ".JPG");
        botoes[i]->setIcon(icon);
        botoes[i]->setIconSize(QSize(300, 300));
    }

    // Atualização de textos

    ui->lblManilha->setText("Manilha: " + game.getManilha().toString().first);
    QPixmap pixmapManilha(":/imagens/cartas_" + game.getManilha().toString().second + "/" + game.getManilha().toString().second + "_" + game.getManilha().toString().first + ".jpg");
    if (pixmapManilha.isNull())
        pixmapManilha.load(":/imagens/cartas_" + game.getManilha().toString().second + "/" + game.getManilha().toString().second + "_" + game.getManilha().toString().first + ".JPG");
    ui->lblManilha->setPixmap(pixmapManilha);
    ui->lblManilha->move(1000,200);

    ui->lblPontos->setText(QString("Nós: %1 - Eles: %2").arg(game.getPontosDupla1()).arg(game.getPontosDupla2()));
    ui->lblTruco->setText(QString("Rodada valendo: %1 ponto(s)").arg(game.getValorRodada()));

    // Indicadores de pontos da rodada
    QStringList circulo = { "circulo_cinza.png", "circulo_cinza.png", "circulo_cinza.png" };
    QStringList circulo1 = { "circulo_cinza.png", "circulo_cinza.png", "circulo_cinza.png" };
    for (int i = 0; i < int(game.getpontosRodadaDupla1().size()); i++) {
        circulo[i] = game.getpontosRodadaDupla1()[i] == 1 ? "circulo_verde.png" : "circulo_vermelho.png";
        circulo1[i] = game.getpontosRodadaDupla1()[i] == 1 ? "circulo_vermelho.png" : "circulo_verde.png";
    }

    QLabel* circuloLabel[3] = { ui->pontoNos1, ui->pontoNos2, ui->pontoNos3 };
    QLabel* circuloLabel1[3] = { ui->pontoEles1, ui->pontoEles2, ui->pontoEles3 };
    for (int i = 0; i < 3; ++i) {
        QPixmap pix(":/imagens/" + circulo[i]);
        QPixmap pix1(":/imagens/" + circulo1[i]);
        circuloLabel[i]->setPixmap(pix);
        circuloLabel1[i]->setPixmap(pix1);
    }

    ui->lblMesa->move(1000, 1000);

    // Ativa botões se for nova rodada
    if (game.getResetButton()) {


        cartasBot1 = { ui->cartaJogador1_1, ui->cartaJogador1_2, ui->cartaJogador1_3 };
        cartasBot2 = { ui->cartaJogador2_1, ui->cartaJogador2_2, ui->cartaJogador2_3 };
        cartasBot3 = { ui->cartaJogador3_1, ui->cartaJogador3_2, ui->cartaJogador3_3 };

        QPixmap cartaAdversario(":/imagens/costas_carta.png");

        // Tamanho padrão das cartas
        const int largura = 80 * 1.6;
        const int altura = 80;
        const int espacamento1 = 10;

        // --- Jogador 1 (esquerda)
        int xJ1 = ui->mesa->x() + 20;
        int yBaseJ1 = 480;

        for (int i = 0; i < 3; ++i) {
            cartasBot1[i]->setGeometry(xJ1, yBaseJ1 - i * (altura + espacamento1), largura, altura);
            cartasBot1[i]->setPixmap(cartaAdversario);
            cartasBot1[i]->setScaledContents(true);
        }

        // --- Jogador 3 (direita)
        int xJ3 = ui->mesa->x() + ui->mesa->width() - largura - 20;
        int yBaseJ3 = 480;

        for (int i = 0; i < 3; ++i) {
            cartasBot3[i]->setGeometry(xJ3, yBaseJ3 - i * (altura + espacamento1), largura, altura);
            cartasBot3[i]->setPixmap(cartaAdversario);
            cartasBot3[i]->setScaledContents(true);
        }

        // --- Jogador 2 (superior)
        int larguraJ2 = 80;
        int alturaJ2 = 80 * 1.6;
        int xBaseJ2 = (this->width() - (3 * larguraJ2 + 2 * espacamento1)) / 2;
        int yJ2 = ui->mesa->y() + 10;

        for (int i = 0; i < 3; ++i) {
            cartasBot2[i]->setGeometry(xBaseJ2 + i * (larguraJ2 + espacamento1), yJ2, larguraJ2, alturaJ2);
            cartasBot2[i]->setPixmap(cartaAdversario);
            cartasBot2[i]->setScaledContents(true);
        }

        for (auto botao : botoes) botao->setEnabled(true);

        QPushButton* botoes[3] = { ui->btnCarta1, ui->btnCarta2, ui->btnCarta3 };
        int larguraCarta = 120;
        int espacamento = 20;
        int xInicial = 560;
        int yInicial = 570;

        for (int i = 0; i < 3; ++i) {
            botoes[i]->setText("");
            botoes[i]->setStyleSheet(
                "QPushButton {"
                "  border-radius: 5px;"
                "  border: 2px solid white;"
                "  background-color: white;"
                "}"
                );
            botoes[i]->move(xInicial + i * (larguraCarta + espacamento), yInicial);
        }
        game.setResetButton(false);
    }

    // Jogar carta se necessário
    if (game.getIdxJogadorVencedor() != 0 && game.getCartasMesa().empty()) {

        qDebug() << "Bot iniciando nova rodada...";

        int qtdBotsFaltantes = int(game.getJogadores().size() - game.getCartasMesa().size());
        executarJogadasBotsEncadeadas(0, qtdBotsFaltantes, game.getIdxJogadorVencedor());
    }

}

void MainWindow::on_btnCarta1_clicked() {
    // Desativa botão imediatamente para evitar múltiplos cliques
    ui->btnCarta1->setEnabled(false);
    ui->btnCarta1->raise();

    // Cria animação
    QPropertyAnimation* animacao = new QPropertyAnimation(ui->btnCarta1, "geometry");
    animacao->setDuration(350);  // duração em milissegundos

    QRect inicio = ui->btnCarta1->geometry();

    // Centro da mesa
    int centroX = ui->mesa->x() + ui->mesa->width() / 2 - ui->btnCarta1->width() / 2;
    int centroY = ui->mesa->y() + ui->mesa->height() / 2 - ui->btnCarta1->height() / 2;
    QRect destino(centroX, centroY, ui->btnCarta1->width(), ui->btnCarta1->height());

    animacao->setStartValue(inicio);
    animacao->setEndValue(destino);

    // Executa animação
    animacao->start(QAbstractAnimation::DeleteWhenStopped);

    // Após animação, realiza jogada
    connect(animacao, &QPropertyAnimation::finished, this, [=]() {
        game.jogarCarta(0, 0);
        int qtdBotsFaltantes = int(game.getJogadores().size() - game.getCartasMesa().size());
        executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
        qDebug()<<"Cartas bot restantes";

    });
}

void MainWindow::on_btnCarta2_clicked() {
    // Desativa botão imediatamente para evitar múltiplos cliques
    ui->btnCarta2->setEnabled(false);

    ui->btnCarta2->raise();
    // Cria animação
    QPropertyAnimation* animacao = new QPropertyAnimation(ui->btnCarta2, "geometry");
    animacao->setDuration(350);  // duração em milissegundos

    QRect inicio = ui->btnCarta2->geometry();

    // Centro da mesa
    int centroX = ui->mesa->x() + ui->mesa->width() / 2 - ui->btnCarta2->width() / 2;
    int centroY = ui->mesa->y() + ui->mesa->height() / 2 - ui->btnCarta2->height() / 2;
    QRect destino(centroX, centroY, ui->btnCarta2->width(), ui->btnCarta2->height());

    animacao->setStartValue(inicio);
    animacao->setEndValue(destino);

    // Executa animação
    animacao->start(QAbstractAnimation::DeleteWhenStopped);

    // Após animação, realiza jogada
    connect(animacao, &QPropertyAnimation::finished, this, [=]() {
        game.jogarCarta(0, 1);
        int qtdBotsFaltantes = int(game.getJogadores().size() - game.getCartasMesa().size());
        executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
    });
}

void MainWindow::on_btnCarta3_clicked() {
    // Desativa botão imediatamente para evitar múltiplos cliques
    ui->btnCarta3->setEnabled(false);

    ui->btnCarta3->raise();
    // Cria animação
    QPropertyAnimation* animacao = new QPropertyAnimation(ui->btnCarta3, "geometry");
    animacao->setDuration(350);  // duração em milissegundos

    QRect inicio = ui->btnCarta3->geometry();

    // Centro da mesa
    int centroX = ui->mesa->x() + ui->mesa->width() / 2 - ui->btnCarta3->width() / 2;
    int centroY = ui->mesa->y() + ui->mesa->height() / 2 - ui->btnCarta3->height() / 2;
    QRect destino(centroX, centroY, ui->btnCarta3->width(), ui->btnCarta3->height());

    animacao->setStartValue(inicio);
    animacao->setEndValue(destino);

    // Executa animação
    animacao->start(QAbstractAnimation::DeleteWhenStopped);

    // Após animação, realiza jogada
    connect(animacao, &QPropertyAnimation::finished, this, [=]() {
        game.jogarCarta(0, 2);
        int qtdBotsFaltantes = int(game.getJogadores().size() - game.getCartasMesa().size());
        executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
    });
}

void MainWindow::animarJogadaBot(QLabel* cartaBot, int idx, std::function<void()> aoTerminar) {
    if (!cartaBot) {
        if (aoTerminar) aoTerminar();
        return;
    }
    qDebug() << "Jogando carta bot:" << cartaBot->objectName().toStdString();

    Carta proxCartaBot = game.botJogarCarta(idx);
    cartaBot->raise();
    QPropertyAnimation* animacao = new QPropertyAnimation(cartaBot, "geometry");
    animacao->setDuration(600); // ou ajuste o tempo desejado

    QRect inicio = cartaBot->geometry();

    // Centro da mesa
    int centroX = ui->mesa->x() + ui->mesa->width() / 2 - cartaBot->width() / 2;
    int centroY = ui->mesa->y() + ui->mesa->height() / 2 - cartaBot->height() / 2;
    QRect destino(centroX, centroY, cartaBot->width(), cartaBot->height());

    QPixmap pixmap(":/imagens/cartas_" + proxCartaBot.toString().second + "/" + proxCartaBot.toString().second + "_" + proxCartaBot.toString().first + ".jpg");
    if (pixmap.isNull())
        pixmap.load(":/imagens/cartas_" + proxCartaBot.toString().second + "/" + proxCartaBot.toString().second + "_" + proxCartaBot.toString().first + ".JPG");

    if(cartaBot->objectName().toStdString()[12] == '1' || cartaBot->objectName().toStdString()[12] == '3'){
        QTransform transform;
        transform.rotate(90); // gira 90º no sentido horário

        QPixmap rotatedPixmap = pixmap.transformed(transform, Qt::SmoothTransformation);
        pixmap = rotatedPixmap;

        centroX = ui->mesa->x() + ui->mesa->width() / 2 - ui->btnCarta1->height() / 2;
        centroY = ui->mesa->y() + ui->mesa->height() / 2 - ui->btnCarta1->width() / 2;
        QRect aux(centroX, centroY, ui->btnCarta1->height(), ui->btnCarta1->width());
        destino = aux;
    } else {
        centroX = ui->mesa->x() + ui->mesa->width() / 2 - ui->btnCarta1->width() / 2;
        centroY = ui->mesa->y() + ui->mesa->height() / 2 - ui->btnCarta1->height() / 2;
        QRect aux(centroX, centroY, ui->btnCarta1->width(), ui->btnCarta1->height());
        destino = aux;
    }

    cartaBot->setPixmap(pixmap);
    animacao->setStartValue(inicio);
    animacao->setEndValue(destino);
    animacao->start(QAbstractAnimation::DeleteWhenStopped);    // Após animação, realiza jogada
    connect(animacao, &QPropertyAnimation::finished, this, [=]() {
        game.jogarCarta(idx, -1);
        atualizarInterface();
        qDebug() << "Carta do bot jogada";
        if (aoTerminar) aoTerminar();
    });

}

void MainWindow::executarJogadasBotsEncadeadas(int botIndex, int totalBots, int idxJogadorVencedor) {
    if (botIndex > totalBots) return;

    int idx = (idxJogadorVencedor + botIndex) % game.getJogadores().size();
    if (idx == 0) return;
    QLabel* cartaAnimada = nullptr;
    int cartaAleatoria = 0;
    int max = std::max({cartasBot1.size(),cartasBot2.size(),cartasBot3.size()});
    // Verificação essencial para evitar crash
    if (max == 0) {
        qDebug() << "Todos os bots ficaram sem cartas. Parando execução.";
        game.setResetButton(true);
        atualizarInterface();
        return;
    }
    do{
        cartaAleatoria = QRandomGenerator::global()->bounded(0, max);
        if (idx == 1) cartaAnimada = cartasBot1[cartaAleatoria];
        else if (idx == 2) cartaAnimada = cartasBot2[cartaAleatoria];
        else if (idx == 3) cartaAnimada = cartasBot3[cartaAleatoria];

    }while(!cartaAnimada);

    if (idx == 1) cartasBot1.erase(cartasBot1.begin() + cartaAleatoria);
    else if (idx == 2) cartasBot2.erase(cartasBot2.begin() + cartaAleatoria);
    else if (idx == 3) cartasBot3.erase(cartasBot3.begin() + cartaAleatoria);

    qDebug()<<"Carta apagada: "<< cartaAnimada->objectName().toStdString();
    qDebug()<<"Tamanho cartasBot1: "<< cartasBot1.size();
    qDebug()<<"Tamanho cartasBot2: "<< cartasBot2.size();
    qDebug()<<"Tamanho cartasBot3: "<< cartasBot3.size();

    if (!cartaAnimada) {
        qDebug() << "Erro: cartaAnimada é nullptr!";
        return;
    }

    animarJogadaBot(cartaAnimada, idx, [=]() {
        executarJogadasBotsEncadeadas(botIndex + 1, totalBots, idxJogadorVencedor);
    });
}

void MainWindow::on_btnTruco_clicked() {
    game.pedirTruco();
    atualizarInterface();
}

void MainWindow::on_criarSala_clicked() {
    qDebug()<<"Botão Criar Sala clicado";

    //game.gerarCodigoSala();
    //QString codigo = game.getCodigoSala();

    //ui->chatBox->setText(codigo);
    QJsonObject command;
    command["type"] = "CRIAR_SALA";
    QJsonObject payload;
    payload["userID"] = "13sd1sa3d456";
    payload["vectorUserID"] = 0;
    command["payload"] = payload;
    cliente->sendCommand(command);
    atualizarInterface();
}

void MainWindow::on_entrarSala_clicked() {
    qDebug()<<"Botão Entrar Sala clicado";

    if(game.verificarCodigoSala(ui->entradaCodigo->text())){
        ui->chatBox->setText("Entrando na sala: " + game.getCodigoSala());
    }else{
        ui->chatBox->setText("Sala não encontrada");
    }

    cliente->connectToServer("192.168.0.162", 5000);
    atualizarInterface();
}

void MainWindow::clientMessageReceived(const QJsonObject &message)
{
    QString type = message["type"].toString();
    QJsonObject payload = message["payload"].toObject();

    qDebug() << (QString("Servidor -> %1: %2").arg(type, QJsonDocument(payload).toJson(QJsonDocument::Compact).constData()));

    if (type == "WELCOME") {
        qDebug() << ("Mensagem de boas-vindas: " + payload["message"].toString());
    } else if (type == "PONG") {
        qint64 clientTimestamp = message["payload"].toObject()["clientTimestamp"].toVariant().toLongLong();
        qint64 serverTimestamp = payload["timestamp"].toVariant().toLongLong();
        qint64 latency = QDateTime::currentMSecsSinceEpoch() - clientTimestamp;
        qDebug() << (QString("Recebido PONG. Latência: %1 ms (Server time: %2)").arg(latency).arg(QDateTime::fromMSecsSinceEpoch(serverTimestamp).toString(Qt::ISODate)));
    } else if (type == "CODIGO_CRIAR_SALA") {
        qDebug() << ("Código criado: " + payload["codigo"].toString());
        ui->chatBox->setText(payload["codigo"].toString());
    } else if (type == "SERVER_STATUS") {
        qDebug() << (QString("Status do Servidor: Hora %1, Uptime %2 segundos.")
                      .arg(payload["serverTime"].toString())
                      .arg(payload["uptimeSeconds"].toInt()));
    } else if (type == "ERROR") {
        qDebug() << ("ERRO do Servidor: " + payload["error"].toString());
    }
    atualizarInterface();
}

void MainWindow::clientError(const QString &errorString)
{
    qDebug()<<("Erro do Cliente: " + errorString);
}


