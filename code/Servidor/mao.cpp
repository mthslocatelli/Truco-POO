
#include "mao.h"

void Hand::deal(Baralho& deck, Jogador& human, JogadorBot& bot) {
    for (int i = 0; i < 3; ++i) {
        human.receberCarta(deck.distribuir());
        bot.receberCarta(deck.distribuir());
    }
}
