#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    atualizarInterface();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::atualizarInterface() {
    auto mao = game.getJogadores()[0]->getMao();
    ui->btnCarta1->setText(mao[0].toString());
    ui->btnCarta2->setText(mao[1].toString());
    ui->btnCarta3->setText(mao[2].toString());

    ui->lblManilha->setText("Manilha: " + game.getManilha().toString());
    ui->lblPontos->setText(QString("Dupla 1: %1 - Dupla 2: %2").arg(game.getPontosDupla1()).arg(game.getPontosDupla2()));
    ui->lblTruco->setText(QString("Rodada valendo: %1 ponto(s)").arg(game.getValorRodada()));

    QString cartasMesa;
    int i = 0;
    for (auto& c : game.getCartasMesa()) {
        cartasMesa += QString("Jogador %1 jogou %2\n").arg(i + 1).arg(c.toString());
        i++;
    }
    ui->lblMesa->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->lblMesa->setText(cartasMesa);
    ui->lblMesa->repaint();

    if(game.getTurnoAnterior() == 12 && game.getTurno() == 0){
        ui ->btnCarta1->setEnabled(true);
        ui ->btnCarta2->setEnabled(true);
        ui ->btnCarta3->setEnabled(true);
    }

}

void MainWindow::on_btnCarta1_clicked() {
    game.jogarCarta(0, 0);
    ui ->btnCarta1->setEnabled(false);
    atualizarInterface();
}
void MainWindow::on_btnCarta2_clicked() {
    game.jogarCarta(0, 1);
    ui ->btnCarta2->setEnabled(false);
    atualizarInterface();
}
void MainWindow::on_btnCarta3_clicked() {
    game.jogarCarta(0, 2);
    ui ->btnCarta3->setEnabled(false);
    atualizarInterface();
}
void MainWindow::on_btnTruco_clicked() {
    game.pedirTruco();
    atualizarInterface();
}
