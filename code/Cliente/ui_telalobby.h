/********************************************************************************
** Form generated from reading UI file 'telalobby.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELALOBBY_H
#define UI_TELALOBBY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelaLobby
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblTituloSala;
    QVBoxLayout *layoutJogadores;
    QLabel *lblJogador1;
    QLabel *lblJogador2;
    QLabel *lblJogador3;
    QLabel *lblJogador4;
    QPushButton *btnIniciarPartida;

    void setupUi(QWidget *TelaLobby)
    {
        if (TelaLobby->objectName().isEmpty())
            TelaLobby->setObjectName("TelaLobby");
        TelaLobby->resize(600, 400);
        verticalLayout = new QVBoxLayout(TelaLobby);
        verticalLayout->setObjectName("verticalLayout");
        lblTituloSala = new QLabel(TelaLobby);
        lblTituloSala->setObjectName("lblTituloSala");
        lblTituloSala->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblTituloSala);

        layoutJogadores = new QVBoxLayout();
        layoutJogadores->setObjectName("layoutJogadores");
        lblJogador1 = new QLabel(TelaLobby);
        lblJogador1->setObjectName("lblJogador1");

        layoutJogadores->addWidget(lblJogador1);

        lblJogador2 = new QLabel(TelaLobby);
        lblJogador2->setObjectName("lblJogador2");

        layoutJogadores->addWidget(lblJogador2);

        lblJogador3 = new QLabel(TelaLobby);
        lblJogador3->setObjectName("lblJogador3");

        layoutJogadores->addWidget(lblJogador3);

        lblJogador4 = new QLabel(TelaLobby);
        lblJogador4->setObjectName("lblJogador4");

        layoutJogadores->addWidget(lblJogador4);


        verticalLayout->addLayout(layoutJogadores);

        btnIniciarPartida = new QPushButton(TelaLobby);
        btnIniciarPartida->setObjectName("btnIniciarPartida");

        verticalLayout->addWidget(btnIniciarPartida);


        retranslateUi(TelaLobby);

        QMetaObject::connectSlotsByName(TelaLobby);
    } // setupUi

    void retranslateUi(QWidget *TelaLobby)
    {
        TelaLobby->setWindowTitle(QCoreApplication::translate("TelaLobby", "Sala de Espera", nullptr));
        lblTituloSala->setText(QCoreApplication::translate("TelaLobby", "Sala C\303\263digo: 123456", nullptr));
        lblJogador1->setText(QCoreApplication::translate("TelaLobby", "Jogador 1", nullptr));
        lblJogador2->setText(QCoreApplication::translate("TelaLobby", "Jogador 2", nullptr));
        lblJogador3->setText(QCoreApplication::translate("TelaLobby", "Jogador 3", nullptr));
        lblJogador4->setText(QCoreApplication::translate("TelaLobby", "Jogador 4", nullptr));
        btnIniciarPartida->setText(QCoreApplication::translate("TelaLobby", "Iniciar Partida", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TelaLobby: public Ui_TelaLobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELALOBBY_H
