#ifndef JOGADORBOT_H
#define JOGADORBOT_H

#include "jogador.h"

class JogadorBot : public Jogador {
public:
    JogadorBot(QString nome = "Bot");

    Carta jogarCarta() override;
    Carta jogarCarta(bool simulacao) override;

private:
    int cartaAtual;
};

#endif // JOGADORBOT_H
