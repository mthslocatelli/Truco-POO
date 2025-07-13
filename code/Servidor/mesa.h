#ifndef MESA_H
#define MESA_H

#include "carta.h"
#include <vector>

class Mesa {
public:
    Mesa();

    void limpar();
    void jogarCarta(int jogadorId, const Carta& carta);

    const Carta& getCartaNaMesa(int jogadorId) const;
    const std::vector<Carta>& getCartasNaMesa() const;

private:
    std::vector<Carta> cartasNaMesa; // índice = jogadorId
};

#endif // MESA_H
