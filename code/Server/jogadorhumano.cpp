#include "JogadorHumano.h"

JogadorHumano::JogadorHumano(QString nome) : Jogador(nome) {}

Carta JogadorHumano::jogarCarta() {
    // Retorno vazio pois é uma interface não usada para humanos diretamente
    return Carta();
}

Carta JogadorHumano::jogarCarta(int idx) {
    Carta carta = mao[idx];  // copia a carta antes de remover
    return carta;
}

std::vector<Carta> JogadorHumano::getMao() const {
    return mao;
}
