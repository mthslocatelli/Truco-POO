#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include "baralho.h"
#include "jogador.h"

class GameEngine {
public:

    static GameEngine* getInstance();

    void iniciarPartida();
    void jogarCarta(int jogadorIdx, int cartaIdx);
    Carta botJogarCarta(int jogadorIdx);
    void avaliarRodada();
    void proximaRodada();
    void pedirTruco();

    const std::vector<Jogador*>& getJogadores() const { return jogadores; }
    const Carta& getManilha() const { return manilha; }
    const std::vector<Carta>& getCartasMesa() const { return cartasMesa; }
    const std::vector<Carta>& getcartasMesaAnimacao() const { return cartasMesaAnimacao; }
    int getPontosDupla1() const { return pontosDupla1; }
    int getPontosDupla2() const { return pontosDupla2; }
    int getValorRodada() const { return valorRodada; }
    const std::vector<int> getpontosRodadaDupla1() const { return pontosRodadaDupla1; }
    const std::vector<int> getpontosRodadaDupla2() const { return pontosRodadaDupla2; }
    int getTurnoAnterior();
    int getTurno();
    bool getNovoJogo();
    int getIdxJogadorVencedor();
    bool getResetButton();
    void setResetButton(bool state);
    Carta getCartaBotJogada() { return cartaBotJogada; }
    void gerarCodigoSala(QString userID);
    bool verificarCodigoSala(QString codigoSala);
    QString getCodigoSala(){return codigoSala;}
    QString entrarSala(QString codigo, QString userID);
    void sairSala(QString userID);
    void setModo(int modo_){modo = modo_;}
    int getModo(){return modo;}
    void addBot(int idx);

    void distribuirCartas();
    void definirManilha();

private:

    GameEngine();                    // Construtor privado
    static GameEngine* instance;     // Ponteiro para a instância

    std::vector<Jogador*> jogadores;
    Baralho baralho;
    Carta manilha;
    Carta cartaBotJogada;
    std::vector<Carta> cartasMesa;
    std::vector<Carta> cartasMesaAnimacao;
    QString codigoSala;

    bool novoJogo;
    int pontosDupla1;
    int pontosDupla2;
    std::vector<int> pontosRodadaDupla1;
    std::vector<int> pontosRodadaDupla2;
    int valorRodada;
    bool trucoPedido;
    int turno;
    int turnoAnterior;
    int rodadaAtual;
    int indiceJogadorVencedor;
    int proxIdxJogador;
    bool resetButton;
    int modo;

};

#endif // GAMEENGINE_H
