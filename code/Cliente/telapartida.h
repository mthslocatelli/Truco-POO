#ifndef TELAPARTIDA_H
#define TELAPARTIDA_H

#include <QMainWindow>
#include "gameengine.h"
#include <QLabel>
#include "clientetruco.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TelaPartida; }
QT_END_NAMESPACE

class TelaPartida : public QMainWindow {
    Q_OBJECT

public:
    TelaPartida(GameEngine* gameEngine, QWidget *parent = nullptr);
    ~TelaPartida();

    void animarJogada(int idxJogador, QString naipe, QString valor, int idxCarta);
    void atualizarInterface();
    void executarJogadasBotsEncadeadas(int botIndex, int totalBots, int idxJogadorVencedor);

signals:
    void sairPartida();
    void cartaJogada(int idxCartaJogada);

private slots:
    void on_btnCarta1_clicked();
    void on_btnCarta2_clicked();
    void on_btnCarta3_clicked();
    void on_btnTruco_clicked();
    void animarJogadaBot(QLabel* cartaBot, int cartaAleatoria ,int idx, std::function<void()> aoTerminar);

private:
    Ui::TelaPartida *ui;
    GameEngine* game;
    std::vector<QLabel*> cartasJogador1;
    std::vector<QLabel*> cartasJogador2;
    std::vector<QLabel*> cartasJogador3;
    ClienteTruco *cliente;

};
#endif // TELAPARTIDA_H
