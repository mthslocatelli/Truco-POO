#include "telaprocurando.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
TelaProcurando::TelaProcurando(QWidget *parent) : QWidget(parent) {
    lblStatus = new QLabel("Procurando jogadores...", this);
    lblStatus->setAlignment(Qt::AlignHCenter);
    btnCancelar = new QPushButton("Cancelar", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lblStatus);
    layout->addWidget(btnCancelar);

    setLayout(layout);

    connect(btnCancelar, &QPushButton::clicked, this, &TelaProcurando::cancelarBusca);
}
