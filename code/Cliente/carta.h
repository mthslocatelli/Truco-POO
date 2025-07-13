#ifndef CARTA_H
#define CARTA_H

#include <QString>

enum Naipe {Paus, Ouros, Copas, Espadas};

class Carta {
public:
    Carta(int valor = 0, Naipe naipe = Paus);
    Carta(QString valor, QString naipe);

    int getValor() const;
    Naipe getNaipe() const;
    std::pair<QString,QString> toString() const;
    int getNaipeValue(const Naipe naipe) const;
    QString getNomeArquivo() const {
        return QString(":/cartas_%1/%1_%2.png").arg(naipe).arg(valor);
    }
    static void setManilha(const Carta& vira);
    static bool isManilha(const Carta& c);
    Carta getCarta(QString valor, QString naipe);

    // Para comparação e regras (ordenar)
    bool operator==(const Carta& other) const;
    bool operator<(const Carta& other) const;
    bool operator>(const Carta& outra) const;

private:
    int valor; // 4 a 12 (no truco paulista, 4 é menor, depois 5... até 12)
    Naipe naipe;
};

#endif // CARTA_H
