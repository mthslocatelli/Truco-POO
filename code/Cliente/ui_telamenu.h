/********************************************************************************
** Form generated from reading UI file 'telamenu.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELAMENU_H
#define UI_TELAMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelaMenu
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *btnCriarSala;
    QPushButton *btnEntrarSala;
    QPushButton *btnEncontrarPartida;
    QPushButton *btnJogarBots;

    void setupUi(QWidget *TelaMenu)
    {
        if (TelaMenu->objectName().isEmpty())
            TelaMenu->setObjectName("TelaMenu");
        TelaMenu->resize(600, 400);
        verticalLayout = new QVBoxLayout(TelaMenu);
        verticalLayout->setObjectName("verticalLayout");
        btnCriarSala = new QPushButton(TelaMenu);
        btnCriarSala->setObjectName("btnCriarSala");
        btnCriarSala->setEnabled(true);

        verticalLayout->addWidget(btnCriarSala, 0, Qt::AlignmentFlag::AlignHCenter);

        btnEntrarSala = new QPushButton(TelaMenu);
        btnEntrarSala->setObjectName("btnEntrarSala");

        verticalLayout->addWidget(btnEntrarSala);

        btnEncontrarPartida = new QPushButton(TelaMenu);
        btnEncontrarPartida->setObjectName("btnEncontrarPartida");

        verticalLayout->addWidget(btnEncontrarPartida);

        btnJogarBots = new QPushButton(TelaMenu);
        btnJogarBots->setObjectName("btnJogarBots");

        verticalLayout->addWidget(btnJogarBots);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);

        retranslateUi(TelaMenu);

        QMetaObject::connectSlotsByName(TelaMenu);
    } // setupUi

    void retranslateUi(QWidget *TelaMenu)
    {
        TelaMenu->setWindowTitle(QCoreApplication::translate("TelaMenu", "Menu Principal", nullptr));
        btnCriarSala->setText(QCoreApplication::translate("TelaMenu", "Criar Sala", nullptr));
        btnEntrarSala->setText(QCoreApplication::translate("TelaMenu", "Entrar em Sala", nullptr));
        btnEncontrarPartida->setText(QCoreApplication::translate("TelaMenu", "Encontrar Partida", nullptr));
        btnJogarBots->setText(QCoreApplication::translate("TelaMenu", "Jogar com Bots", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TelaMenu: public Ui_TelaMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELAMENU_H
