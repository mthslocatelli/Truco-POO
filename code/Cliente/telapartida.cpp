#include "telapartida.h"
#include "mainwindow.h"
#include "ui_telapartida.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QTimer>
#include <QStackedWidget>



TelaPartida::TelaPartida(GameEngine* gameEngine, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TelaPartida), game(gameEngine) {
    ui->setupUi(this);
    this->showFullScreen();

    connect(ui->sairSala, &QPushButton::clicked, this, &TelaPartida::sairPartida);

    cartasJogador1 = { ui->cartaJogador1_1, ui->cartaJogador1_2, ui->cartaJogador1_3 };
    cartasJogador2 = { ui->cartaJogador2_1, ui->cartaJogador2_2, ui->cartaJogador2_3 };
    cartasJogador3 = { ui->cartaJogador3_1, ui->cartaJogador3_2, ui->cartaJogador3_3 };

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
        botoes[i]->setObjectName("btnCarta" + QString::number(i));
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

TelaPartida::~TelaPartida() {
    delete ui;
}

void TelaPartida::atualizarInterface() {

    qDebug() << "Atualizando interface...";

    auto mao = game->getJogadores()[game->getMeuIdx()]->getMao();
    qDebug() << "IdxAtual: "<< game->getIdxAtual();

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
        botoes[i]->setObjectName("btnCarta" + QString::number(i));
        botoes[i]->setIconSize(QSize(300, 300));
        // Verifica se é a vez do jogador
        if (game->getMeuIdx() == game->getIdxAtual()) {
            bool cartaJaNaMesa = false;
            // Verifica se essa carta da mão já está na mesa
            for (const auto& c : game->getCartasMesa()) {
                if (naipe == c.toString().second && valor == c.toString().first) {
                    cartaJaNaMesa = true;
                    break;
                }
            }
            botoes[i]->setEnabled(!cartaJaNaMesa);
            qDebug() << (cartaJaNaMesa ? "Desativando" : "Ativando") << "o botão" << i;
        } else {
            botoes[i]->setEnabled(false);
            qDebug() << "Não é sua vez. Desativando o botão" << i;
        }
    }

    // Atualização de textos

    ui->lblManilha->setText("Manilha: " + game->getManilha().toString().first);
    QPixmap pixmapManilha(":/imagens/cartas_" + game->getManilha().toString().second + "/" + game->getManilha().toString().second + "_" + game->getManilha().toString().first + ".jpg");
    if (pixmapManilha.isNull())
        pixmapManilha.load(":/imagens/cartas_" + game->getManilha().toString().second + "/" + game->getManilha().toString().second + "_" + game->getManilha().toString().first + ".JPG");
    ui->lblManilha->setPixmap(pixmapManilha);
    ui->lblManilha->move(1000,200);

    ui->lblPontos->setText(QString("Nós: %1 - Eles: %2").arg(game->getPontosDupla1()).arg(game->getPontosDupla2()));
    ui->lblTruco->setText(QString("Rodada valendo: %1 ponto(s)").arg(game->getValorRodada()));

    // Indicadores de pontos da rodada
    QStringList circulo = { "circulo_cinza.png", "circulo_cinza.png", "circulo_cinza.png" };
    QStringList circulo1 = { "circulo_cinza.png", "circulo_cinza.png", "circulo_cinza.png" };
    for (int i = 0; i < int(game->getpontosRodadaDupla1().size()); i++) {
        circulo[i] = game->getpontosRodadaDupla1()[i] == 1 ? "circulo_verde.png" : "circulo_vermelho.png";
        circulo1[i] = game->getpontosRodadaDupla1()[i] == 1 ? "circulo_vermelho.png" : "circulo_verde.png";
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
    if (game->getResetButton()) {


        cartasJogador1 = { ui->cartaJogador1_1, ui->cartaJogador1_2, ui->cartaJogador1_3 };
        cartasJogador2 = { ui->cartaJogador2_1, ui->cartaJogador2_2, ui->cartaJogador2_3 };
        cartasJogador3 = { ui->cartaJogador3_1, ui->cartaJogador3_2, ui->cartaJogador3_3 };

        QPixmap cartaAdversario(":/imagens/costas_carta.png");

        // Tamanho padrão das cartas
        const int largura = 80 * 1.6;
        const int altura = 80;
        const int espacamento1 = 10;

        // --- Jogador 1 (esquerda)
        int xJ1 = ui->mesa->x() + 20;
        int yBaseJ1 = 480;

        for (int i = 0; i < 3; ++i) {
            cartasJogador1[i]->setGeometry(xJ1, yBaseJ1 - i * (altura + espacamento1), largura, altura);
            cartasJogador1[i]->setPixmap(cartaAdversario);
            cartasJogador1[i]->setScaledContents(true);
        }

        // --- Jogador 3 (direita)
        int xJ3 = ui->mesa->x() + ui->mesa->width() - largura - 20;
        int yBaseJ3 = 480;

        for (int i = 0; i < 3; ++i) {
            cartasJogador3[i]->setGeometry(xJ3, yBaseJ3 - i * (altura + espacamento1), largura, altura);
            cartasJogador3[i]->setPixmap(cartaAdversario);
            cartasJogador3[i]->setScaledContents(true);
        }

        // --- Jogador 2 (superior)
        int larguraJ2 = 80;
        int alturaJ2 = 80 * 1.6;
        int xBaseJ2 = (this->width() - (3 * larguraJ2 + 2 * espacamento1)) / 2;
        int yJ2 = ui->mesa->y() + 10;

        for (int i = 0; i < 3; ++i) {
            cartasJogador2[i]->setGeometry(xBaseJ2 + i * (larguraJ2 + espacamento1), yJ2, larguraJ2, alturaJ2);
            cartasJogador2[i]->setPixmap(cartaAdversario);
            cartasJogador2[i]->setScaledContents(true);
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
        game->setResetButton(false);
    }

    // Jogar carta se necessário
    if (game->getIdxJogadorVencedor() != 0 && game->getCartasMesa().empty() && game->getModo() == 1) {

        qDebug() << "Bot iniciando nova rodada...";

        int qtdBotsFaltantes = int(game->getJogadores().size() - game->getCartasMesa().size());
        executarJogadasBotsEncadeadas(0, qtdBotsFaltantes, game->getIdxJogadorVencedor());
    }

}

void TelaPartida::on_btnCarta1_clicked() {

    game->jogarCarta(game->getMeuIdx(), 0);
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
        emit cartaJogada(0);
        if(game->getModo() == 1){
        int qtdBotsFaltantes = int(game->getJogadores().size() - game->getCartasMesa().size());
        executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
        }

    });
}

void TelaPartida::on_btnCarta2_clicked() {

    game->jogarCarta(game->getMeuIdx(), 1);
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
        emit cartaJogada(1);
        if(game->getModo() == 1){
            int qtdBotsFaltantes = int(game->getJogadores().size() - game->getCartasMesa().size());
            executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
        }
    });
}

void TelaPartida::on_btnCarta3_clicked() {

    game->jogarCarta(game->getMeuIdx(), 2);
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
        emit cartaJogada(2);
        if(game->getModo() == 1){
            int qtdBotsFaltantes = int(game->getJogadores().size() - game->getCartasMesa().size());
            executarJogadasBotsEncadeadas(1, qtdBotsFaltantes, 0);
        }
    });
}

void TelaPartida::animarJogadaBot(QLabel* cartaBot, int cartaAleatoria, int idx, std::function<void()> aoTerminar) {
    if (!cartaBot) {
        if (aoTerminar) aoTerminar();
        return;
    }
    qDebug() << "Jogando carta bot:" << cartaBot->objectName().toStdString();
    game->jogarCarta(idx, -1);

    Carta proxCartaBot = game->botJogarCarta(idx);
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
        emit cartaJogada(cartaAleatoria);
        atualizarInterface();
        qDebug() << "Carta do bot jogada";
        if (aoTerminar) aoTerminar();
    });

}

void TelaPartida::animarJogada(int idxJogador, QString naipe, QString valor, int idxCarta) {
    qDebug() << "Animando jogada player...";
    int idxJogadorRelativo = (idxJogador - game->getMeuIdx() + 4)%4;
    if(idxJogadorRelativo  == 0) return;
    QLabel* cartaAnimada = nullptr;
    int cartaAleatoria = 0;
    int max = std::max({cartasJogador1.size(),cartasJogador2.size(),cartasJogador3.size()});
    // Verificação essencial para evitar crash
    if (max == 0) {
        game->setResetButton(true);
        atualizarInterface();
        return;
    }
    cartaAleatoria = idxCarta;
    if (idxJogadorRelativo == 1) cartaAnimada = cartasJogador1[cartaAleatoria];
    else if (idxJogadorRelativo == 2) cartaAnimada = cartasJogador2[cartaAleatoria];
    else if (idxJogadorRelativo == 3) cartaAnimada = cartasJogador3[cartaAleatoria];

    qDebug() << "Jogando carta Jogador"<< idxJogadorRelativo <<":"<< cartaAnimada->objectName().toStdString();

    cartaAnimada->raise();
    QPropertyAnimation* animacao = new QPropertyAnimation(cartaAnimada, "geometry");
    animacao->setDuration(600); // ou ajuste o tempo desejado

    QRect inicio = cartaAnimada->geometry();

    // Centro da mesa
    int centroX = ui->mesa->x() + ui->mesa->width() / 2 - cartaAnimada->width() / 2;
    int centroY = ui->mesa->y() + ui->mesa->height() / 2 - cartaAnimada->height() / 2;
    QRect destino(centroX, centroY, cartaAnimada->width(), cartaAnimada->height());

    QPixmap pixmap(":/imagens/cartas_" + naipe + "/" + naipe + "_" + valor + ".jpg");
    if (pixmap.isNull())
        pixmap.load(":/imagens/cartas_" + naipe + "/" + naipe + "_" + valor + ".JPG");

    if(cartaAnimada->objectName().toStdString()[12] == '1' || cartaAnimada->objectName().toStdString()[12] == '3'){
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

    cartaAnimada->setPixmap(pixmap);
    animacao->setStartValue(inicio);
    animacao->setEndValue(destino);
    animacao->start(QAbstractAnimation::DeleteWhenStopped);    // Após animação, realiza jogada
    connect(animacao, &QPropertyAnimation::finished, this, [=]() {
        game->jogarCarta(idxJogador, idxCarta);
        atualizarInterface();
    });

}

void TelaPartida::executarJogadasBotsEncadeadas(int botIndex, int totalBots, int idxJogadorVencedor) {

    int idx = (idxJogadorVencedor + botIndex) % game->getJogadores().size();
    qDebug() << "idx:" << idx <<"botIndex:"<<botIndex;
    if (botIndex > totalBots) {
        if (idx == 0) {
            qDebug() << "Reativando botões do jogador humano.";
            ui->btnCarta1->setEnabled(true);
            ui->btnCarta2->setEnabled(true);
            ui->btnCarta3->setEnabled(true);
        }
        return;
    }

    QLabel* cartaAnimada = nullptr;
    int cartaAleatoria = 0;
    int max = std::max({cartasJogador1.size(),cartasJogador2.size(),cartasJogador3.size()});
    // Verificação essencial para evitar crash
    if (max == 0) {
        qDebug() << "Todos os bots ficaram sem cartas. Parando execução.";
        game->setResetButton(true);
        atualizarInterface();
        return;
    }
    do{
        cartaAleatoria = QRandomGenerator::global()->bounded(0, max);
        if (idx == 1) cartaAnimada = cartasJogador1[cartaAleatoria];
        else if (idx == 2) cartaAnimada = cartasJogador2[cartaAleatoria];
        else if (idx == 3) cartaAnimada = cartasJogador3[cartaAleatoria];

    }while(!cartaAnimada);

    if (idx == 1) cartasJogador1.erase(cartasJogador1.begin() + cartaAleatoria);
    else if (idx == 2) cartasJogador2.erase(cartasJogador2.begin() + cartaAleatoria);
    else if (idx == 3) cartasJogador3.erase(cartasJogador3.begin() + cartaAleatoria);

    if (!cartaAnimada) {
        qDebug() << "Erro: cartaAnimada é nullptr!";
        return;
    }

    animarJogadaBot(cartaAnimada, cartaAleatoria, idx, [=]() {
        executarJogadasBotsEncadeadas(botIndex + 1, totalBots, idxJogadorVencedor);
    });
}

void TelaPartida::on_btnTruco_clicked() {
    game->pedirTruco();
    atualizarInterface();
}
