#include "telainicial.h"
#include "telalogin.h"
#include "ui_telainicial.h"

TelaInicial::TelaInicial(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TelaInicial){
    setWindowTitle("Truco Paulista - Inicial");
    ui->setupUi(this);
    this->showFullScreen();

    botaoLogin = new QPushButton("Login", this);
    botaoLogin->setGeometry(100, 80, 100, 40);
    ui->botaoLogin->setGeometry(QRect(this->width()/2-ui->botaoLogin->width()/2, this->height()/2-ui->botaoLogin->height()/2, 100, 40));

    connect(botaoLogin, &QPushButton::clicked, this, &TelaInicial::loginClicado);
}

TelaInicial::~TelaInicial() {
    delete ui;
}

void TelaInicial::on_botaoLogin_clicked()
{
    this->close();
    TelaLogin w;
    w.show();
}

