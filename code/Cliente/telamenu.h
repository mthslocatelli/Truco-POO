#ifndef TELAMENU_H
#define TELAMENU_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>

class QPushButton;

class TelaMenu : public QWidget {
    Q_OBJECT

public:
    explicit TelaMenu(QWidget *parent = nullptr);

signals:
    void procurarJogo();
    void jogarBot();
    void entrarSala();
    void iniciarLobby();
    void sair();


private:
    QPushButton* btnIniciar;
    QPushButton* btnSair;
    QPushButton* btnCriarSala;
    QPushButton* btnEntrarSala;
    QPushButton* btnJogarBot;
    QPushButton* btnConfirmaCodigo;
    QLineEdit* txtEdit;
    QVBoxLayout* layout;

};

#endif // TELAMENU_H
