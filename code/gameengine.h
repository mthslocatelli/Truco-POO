#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include "baralho.h"
#include "jogador.h"

class GameEngine {
public:
    GameEngine();
    void iniciarPartida();
    void jogarCarta(int jogadorIdx, int cartaIdx);
    void avaliarRodada();
    void proximaRodada();
    void pedirTruco();

    const std::vector<Jogador*>& getJogadores() const { return jogadores; }
    const Carta& getManilha() const { return manilha; }
    const std::vector<Carta>& getCartasMesa() const { return cartasMesa; }
    int getPontosDupla1() const { return pontosDupla1; }
    int getPontosDupla2() const { return pontosDupla2; }
    int getValorRodada() const { return valorRodada; }
    int getTurnoAnterior();
    int getTurno();
    bool getNovoJogo();

private:
    std::vector<Jogador*> jogadores;
    Baralho baralho;
    Carta manilha;
    std::vector<Carta> cartasMesa;

    bool novoJogo;
    int pontosDupla1;
    int pontosDupla2;
    std::vector<int> pontosRodada;
    int valorRodada;
    bool trucoPedido;
    int turno;
    int turnoAnterior;
    int rodadaAtual;

    void distribuirCartas();
    void definirManilha();
};

#endif // GAMEENGINE_H
