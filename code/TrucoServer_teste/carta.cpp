#include "carta.h"
#include <QDebug>

Carta::Carta(int v, Naipe n) : valor(v), naipe(n) {}

static int valorManilhaGlobal = -1;  // variável global interna

int Carta::getValor() const { return valor; }
Naipe Carta::getNaipe() const { return naipe; }

std::pair<QString,QString> Carta::toString() const {
    QString strValor;
    switch (valor) {
    case 1: strValor = "1"; break;
    case 2: strValor = "2"; break;
    case 3: strValor = "3"; break;
    case 4: strValor = "4"; break;
    case 5: strValor = "5"; break;
    case 6: strValor = "6"; break;
    case 7: strValor = "7"; break;
    case 10: strValor = "10"; break; // Dama
    case 11: strValor = "11"; break; // Valete
    case 12: strValor = "12"; break; // Rei
    default: strValor = QString::number(valor);
    }
    QString strNaipe;
    switch (naipe) {
    case Paus: strNaipe = "paus"; break;
    case Ouros: strNaipe = "ouros"; break;
    case Copas: strNaipe = "copas"; break;
    case Espadas: strNaipe = "espadas"; break;
    }
    return {strValor, strNaipe};
}

bool Carta::operator==(const Carta& other) const {
    return valor == other.valor && naipe == other.naipe;
}

int Carta::getNaipeValue(const Naipe naipe) const{
    switch (naipe) {
    case Paus:  return 3; break;
    case Ouros: return 0; break;
    case Copas: return 2; break;
    case Espadas: return 1; break;
    }
    return -1;
}

void Carta::setManilha(const Carta& vira) {
    if((vira.getValor() % 13) + 1 == 8){
            valorManilhaGlobal = 10;
    }else{
        valorManilhaGlobal = (vira.getValor() % 13) + 1;
    }

    qDebug() << "Manilha definida como:" << valorManilhaGlobal;
}

bool Carta::isManilha(const Carta& c) {
    return c.getValor() == valorManilhaGlobal;
}

bool Carta::operator>(const Carta& outra) const {
    bool thisIsManilha = isManilha(*this);
    bool outraIsManilha = isManilha(outra);

    // 🥇 Prioridade: manilhas
    if (thisIsManilha && !outraIsManilha) return true;
    if (!thisIsManilha && outraIsManilha) return false;

    // 🥈 Prioridade: 3 > 2 > 1
    auto valorForca = [](int v) -> int {
        if (v == 3) return 1003;
        if (v == 2) return 1002;
        if (v == 1) return 1001;
        return v;
    };

    int thisForca = valorForca(this->valor);
    int outraForca = valorForca(outra.valor);

    if (thisForca == outraForca) {
        // 🥉 Desempate: naipe
        return getNaipeValue(this->naipe) > getNaipeValue(outra.naipe);
    }

    return thisForca > outraForca;
}

// Defina operador < para uso interno, mas lembre-se que truco tem regra própria
bool Carta::operator<(const Carta& other) const {
    return valor < other.valor;
}
