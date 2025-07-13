
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientetruco.h"
#include "gameengine.h"

class QStackedWidget;
class TelaMenu;
class TelaLobby;
class TelaProcurando;
class TelaPartida;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString carregarOuGerarUUID();

signals:

    void atualizarLobby();
    void cartaJogada(int idxJogador, QString cartaValor, QString cartaNaipe, int idxCarta);

private slots:

    void clientMessageReceived(const QJsonObject &message);
    void clientError(const QString &errorString);


private:
    QStackedWidget* stackedWidget;
    TelaMenu* telaMenu;
    TelaLobby* telaLobby;
    TelaProcurando* telaProcurando;
    TelaPartida* telaPartida;
    ClienteTruco *cliente;
    QWidget* previousPage;
    GameEngine* game;

    void mostrarPopup(QString mensagem, int duracaoMs);
    void conectarSinais();
};

#endif // MAINWINDOW_H
