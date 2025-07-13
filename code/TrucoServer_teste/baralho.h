#ifndef BARALHO_H
#define BARALHO_H

#include <vector>
#include "carta.h"

class Baralho {
public:
    Baralho();

    void embaralhar();
    Carta comprarCarta();
    int cartasRestantes() const;

private:
    std::vector<Carta> cartas;
    int posTopo;
};

#endif // BARALHO_H
