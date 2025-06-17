#ifndef JOGADOR_H
#define JOGADOR_H

#include <vector>
#include "carta.h"
#include <QString>

class Jogador {
public:
    Jogador(QString nome = "Jogador");
    virtual ~Jogador() {}

    virtual Carta jogarCarta() = 0;
    virtual Carta jogarCarta(int idx) { return jogarCarta(); }

    void receberCarta(Carta c);
    void limparMao();  // ✅ Adicione esta declaração

    const std::vector<Carta>& getMao() const;  // ✅ Corrija para const referência
    QString getNome() const;

protected:
    QString nome;
    std::vector<Carta> mao;
};

#endif
