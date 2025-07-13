#include "baralho.h"
#include <algorithm>
#include <random>
#include <chrono>

Baralho::Baralho() {
    cartas.clear();
    // Cartas usadas no Truco Paulista: valores de 4 a 7 e 10 a 12 em cada naipe
    int valores[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};
    for (int v : valores) {
        cartas.push_back(Carta(v, Paus));
        cartas.push_back(Carta(v, Ouros));
        cartas.push_back(Carta(v, Copas));
        cartas.push_back(Carta(v, Espadas));
    }
    posTopo = 0;
}

void Baralho::embaralhar() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cartas.begin(), cartas.end(), std::default_random_engine(seed));
    posTopo = 0;
}

Carta Baralho::setCarta(QString valor, QString naipe){
    int valor_ = valor.toInt();
    Naipe naipe_;
    if(naipe=="paus") naipe_ = Paus;
    if(naipe=="copas") naipe_ = Copas;
    if(naipe=="espadas") naipe_ = Espadas;
    if(naipe=="ouros") naipe_ = Ouros;
    cartas.erase(std::remove(cartas.begin(), cartas.end(), Carta(valor_, naipe_)), cartas.end());
    return Carta(valor_, naipe_);
}

Carta Baralho::comprarCarta() {
    if (posTopo >= (int)cartas.size()) {
        // Baralho vazio, retornamos carta default inválida
        return Carta();
    }
    return cartas[posTopo++];
}

int Baralho::cartasRestantes() const {
    return cartas.size() - posTopo;
}
