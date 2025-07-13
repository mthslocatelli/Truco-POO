#ifndef TELAPROCURANDO_H
#define TELAPROCURANDO_H

#include <QWidget>

class QPushButton;
class QLabel;

class TelaProcurando : public QWidget {
    Q_OBJECT

public:
    explicit TelaProcurando(QWidget *parent = nullptr);

signals:
    void entrarPartida();
    void cancelarBusca();

private:
    QLabel* lblStatus;
    QPushButton* btnCancelar;
};

#endif // TELAPROCURANDO_H
