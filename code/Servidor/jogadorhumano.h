#ifndef JOGADORHUMANO_H
#define JOGADORHUMANO_H

#include "Jogador.h"

class JogadorHumano : public Jogador {
public:
    JogadorHumano(QString nome);

    Carta jogarCarta() override; // Implementação obrigatória
    Carta jogarCarta(int idx) override;

    std::vector<Carta> getMao() const;



private:

};

#endif // JOGADORHUMANO_H
