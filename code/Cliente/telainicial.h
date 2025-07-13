#ifndef TELAINICIAL_H
#define TELAINICIAL_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class TelaInicial; }
QT_END_NAMESPACE

class TelaInicial : public QMainWindow {
    Q_OBJECT

public:
    TelaInicial(QWidget *parent = nullptr);
    ~TelaInicial();

signals:
    void loginClicado();

private slots:
    void on_botaoLogin_clicked();

private:
    Ui::TelaInicial *ui;
    QPushButton *botaoLogin;
};

#endif // TELAINICIAL_H
