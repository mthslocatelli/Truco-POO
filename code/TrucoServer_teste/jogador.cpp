#include "jogador.h"

Jogador::Jogador(QString nome) : nome(nome) {}

void Jogador::receberCarta(Carta c) {
    mao.push_back(c);
}

void Jogador::limparMao() {  // ✅ Agora está declarado corretamente
    mao.clear();
}

const std::vector<Carta>& Jogador::getMao() const {  // ✅ Corrigido tipo
    return mao;
}

QString Jogador::getNome() const {
    return nome;
}
