#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameengine.h"
#include <QLabel>
#include "clientetruco.h"
#include "servidortruco.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCarta1_clicked();
    void on_btnCarta2_clicked();
    void on_btnCarta3_clicked();
    void on_btnTruco_clicked();
    void on_criarSala_clicked();
    void on_entrarSala_clicked();
    void animarJogadaBot(QLabel* cartaBot, int idx, std::function<void()> aoTerminar);
    void executarJogadasBotsEncadeadas(int botIndex, int totalBots, int idxJogadorVencedor);
    void clientMessageReceived(const QJsonObject &message);
    void clientError(const QString &errorString);

private:
    Ui::MainWindow *ui;
    GameEngine game;
    void atualizarInterface();
    std::vector<QLabel*> cartasBot1;
    std::vector<QLabel*> cartasBot2;
    std::vector<QLabel*> cartasBot3;
    ClienteTruco *cliente;
    ServidorTruco *servidor;

};
#endif // MAINWINDOW_H
