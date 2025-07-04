#include "jogadorbot.h"

JogadorBot::JogadorBot(QString n) : Jogador(n), cartaAtual(0) {}

Carta JogadorBot::jogarCarta() {
    return Carta();
}

Carta JogadorBot::jogarCarta(bool simulacao) {
    if (cartaAtual >= (int)mao.size())
        return Carta(); // carta inválida

    // Estratégia simples: joga a carta mais forte
    int indiceMaior = 0;
    for (int i = 1; i < (int)mao.size(); ++i) {
        if (mao[i].getValor() > mao[indiceMaior].getValor()) {
            indiceMaior = i;
        }
    }
    Carta escolhida = mao[indiceMaior];
    if(!simulacao) mao.erase(mao.begin() + indiceMaior);
    return escolhida;
}
