#include "mesa.h"

Mesa::Mesa() {
    cartasNaMesa.resize(4, Carta());
}

void Mesa::limpar() {
    for (int i=0; i<4; i++) {
        cartasNaMesa[i] = Carta();
    }
}

void Mesa::jogarCarta(int jogadorId, const Carta &carta) {
    if (jogadorId >= 0 && jogadorId < 4) {
        cartasNaMesa[jogadorId] = carta;
    }
}

const Carta& Mesa::getCartaNaMesa(int jogadorId) const {
    if (jogadorId >= 0 && jogadorId < 4) {
        return cartasNaMesa[jogadorId];
    }
    static Carta cartaVazia;
    return cartaVazia;
}

const std::vector<Carta>& Mesa::getCartasNaMesa() const {
    return cartasNaMesa;
}
