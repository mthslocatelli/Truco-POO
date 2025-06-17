#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameengine.h"

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

private:
    Ui::MainWindow *ui;
    GameEngine game;
    void atualizarInterface();
};
#endif // MAINWINDOW_H
