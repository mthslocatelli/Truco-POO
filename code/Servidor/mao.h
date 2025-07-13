
#ifndef MAO_H
#define MAO_H

#include "baralho.h"
#include "jogador.h"
#include "jogadorbot.h"

class Hand {
public:
    void deal(Baralho& deck, Jogador& human, JogadorBot& bot);
};

#endif // MAO_H
