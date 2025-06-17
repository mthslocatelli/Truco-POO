#include "gameengine.h"
#include "jogadorhumano.h"
#include "jogadorbot.h"
#include <QDebug>

GameEngine::GameEngine() {
    jogadores.push_back(new JogadorHumano("Você"));
    jogadores.push_back(new JogadorBot("Bot 1"));
    jogadores.push_back(new JogadorBot("Bot 2"));
    jogadores.push_back(new JogadorBot("Bot 3"));
    iniciarPartida();
}

void GameEngine::iniciarPartida() {
    pontosDupla1 = 0;
    pontosDupla2 = 0;
    valorRodada = 1;
    rodadaAtual = 0;
    trucoPedido = false;
    turno = 0;
    turnoAnterior = 0;
    baralho.embaralhar();
    distribuirCartas();
    definirManilha();
    cartasMesa.clear();
    pontosRodada = {0, 0};
}

void GameEngine::distribuirCartas() {
    for (auto& j : jogadores) {
        j->limparMao();
    }

    for (int i = 0; i < 3; ++i) {
        for (auto& j : jogadores) {
            j->receberCarta(baralho.comprarCarta());
        }
    }
}

void GameEngine::definirManilha() {
    manilha = baralho.comprarCarta();
    Carta::setManilha(manilha);
}

void GameEngine::jogarCarta(int jogadorIdx, int cartaIdx) {
    auto carta = jogadores[jogadorIdx]->jogarCarta(cartaIdx);
    cartasMesa.push_back(carta);
    turno++;

    for (int i = 1; i <= int(jogadores.size() - 1); ++i) {
        auto c = jogadores[i]->jogarCarta();
        cartasMesa.push_back(c);
        turno++;
    }

    for (int i = 0; i <= int(cartasMesa.size() - 1); ++i) {
        qDebug("Carta na mesa: %d - Indice: %d",cartasMesa[i].getValor(), i);
    }

    if (turno % 4 == 0) {
        avaliarRodada();
        cartasMesa.clear();
    }
}

int GameEngine::getTurnoAnterior(){
    return turnoAnterior;
}

int GameEngine::getTurno(){
    return turno;
}

bool GameEngine::getNovoJogo(){
    return novoJogo;
}
void GameEngine::avaliarRodada() {
    qDebug("Rodada sendo avaliada...");
    int vencedor = 0;
    Carta maior = cartasMesa[0];

    for (int i = 0; i < int(cartasMesa.size()); ++i) {
        if (cartasMesa[i] > maior) {
            maior = cartasMesa[i];
            vencedor = i;

        }
    }

    if (vencedor % 2 == 0){
        pontosRodada[0] ++;
        qDebug("Carta vencedora: %d", cartasMesa[vencedor].getValor());
    }
    else{
        pontosRodada[1] ++;
        qDebug("Carta vencedora: %d", cartasMesa[vencedor].getValor());
    }

    if (pontosRodada[0] + pontosRodada[1] == 3){
        if (pontosRodada[0] == 2) pontosDupla1 += valorRodada; else pontosDupla2 += valorRodada;
        cartasMesa.clear();
        turnoAnterior = turno;
        turno = 0;
        pontosRodada[1] = 0;
        proximaRodada();
    }
}

void GameEngine::proximaRodada() {
    if (pontosDupla1 >= 12 || pontosDupla2 >= 12) {
        // fim do jogo
        qDebug("Fim de jogo");
        iniciarPartida();
        novoJogo = true;
        return;
    }
    qDebug("Próxima rodada");
    novoJogo = false;
    pontosRodada = {0,0};
    rodadaAtual++;
    baralho.embaralhar();
    distribuirCartas();
    definirManilha();
    trucoPedido = false;
    valorRodada = 1;
}

void GameEngine::pedirTruco() {
    if (!trucoPedido) {
        trucoPedido = true;
        valorRodada = 3;
    } else if (valorRodada == 3) {
        valorRodada = 6;
    } else if (valorRodada == 6) {
        valorRodada = 9;
    } else {
        valorRodada = 12;
    }
}
