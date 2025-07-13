#include "telalobby.h"
#include "gameengine.h"
#include <QVBoxLayout>

TelaLobby::TelaLobby(QWidget *parent) : QWidget(parent) {
    lblCodigoSala = new QLabel("Código da Sala:", this);
    lblCodigoSala->setObjectName("lblCodigoSala");

    lblInfoQtdJogadores = new QLabel("Jogadores na sala: 1", this);
    lblInfoJogadores = new QLabel("Jogador 1: " + GameEngine::getInstance()->getJogadores()[0]->getNome(), this);

    btnEntrarSala = new QPushButton("Começar Jogo", this);
    btnVoltar = new QPushButton("Voltar ao Menu", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lblCodigoSala);
    layout->addWidget(lblInfoQtdJogadores);  // Adiciona a label de jogadores
    layout->addWidget(lblInfoJogadores);  // Adiciona a label de jogadores
    layout->addWidget(btnEntrarSala);
    layout->addWidget(btnVoltar);

    setLayout(layout);

    connect(btnEntrarSala, &QPushButton::clicked, this, &TelaLobby::iniciarPartida);
    connect(btnVoltar, &QPushButton::clicked, this, &TelaLobby::voltarMenu);
}

void TelaLobby::atualizarJogadores() {

    listaJogadores = GameEngine::getInstance()->getJogadores();

    int quantidade = listaJogadores.size();
    QString texto;

    for (int i = 0; i < quantidade; ++i) {
        texto += QString("Jogador %1: %2\n").arg(i + 1).arg(listaJogadores.at(i)->getNome());
    }

    lblInfoJogadores->setText(texto);
}
