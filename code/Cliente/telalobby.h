#ifndef TELALOBBY_H
#define TELALOBBY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "jogador.h"

class QPushButton;
class QLabel;

class TelaLobby : public QWidget {
    Q_OBJECT

public:
    explicit TelaLobby(QWidget *parent = nullptr);

signals:
    void iniciarPartida();
    void voltarMenu();

public slots:
    void atualizarJogadores();

private:
    QPushButton* btnEntrarSala;
    QPushButton* btnVoltar;
    QLabel* lblCodigoSala;
    QLabel* lblInfoJogadores;  // Label para mostrar quantidade e IDs
    QLabel* lblInfoQtdJogadores;
    std::vector<Jogador*> listaJogadores;
};

#endif // TELALOBBY_H
