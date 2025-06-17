#include "JogadorHumano.h"

JogadorHumano::JogadorHumano(QString nome) : Jogador(nome) {}

Carta JogadorHumano::jogarCarta() {
    // Retorno vazio pois é uma interface não usada para humanos diretamente
    return Carta();
}

Carta JogadorHumano::jogarCarta(int idx) {
    return Carta();
}

std::vector<Carta> JogadorHumano::getMao() const {
    return mao;
}
