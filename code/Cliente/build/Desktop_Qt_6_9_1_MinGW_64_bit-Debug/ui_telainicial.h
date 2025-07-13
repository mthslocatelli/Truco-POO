/********************************************************************************
** Form generated from reading UI file 'telainicial.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELAINICIAL_H
#define UI_TELAINICIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelaInicial
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *spacerTop;
    QPushButton *botaoLogin;
    QSpacerItem *spacerBottom;

    void setupUi(QWidget *TelaInicial)
    {
        if (TelaInicial->objectName().isEmpty())
            TelaInicial->setObjectName("TelaInicial");
        TelaInicial->resize(222, 296);
        verticalLayout = new QVBoxLayout(TelaInicial);
        verticalLayout->setObjectName("verticalLayout");
        spacerTop = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(spacerTop);

        botaoLogin = new QPushButton(TelaInicial);
        botaoLogin->setObjectName("botaoLogin");
        botaoLogin->setMinimumSize(QSize(200, 60));
        botaoLogin->setMaximumSize(QSize(300, 100));
        QFont font;
        font.setPointSize(16);
        botaoLogin->setFont(font);

        verticalLayout->addWidget(botaoLogin);

        spacerBottom = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(spacerBottom);


        retranslateUi(TelaInicial);

        QMetaObject::connectSlotsByName(TelaInicial);
    } // setupUi

    void retranslateUi(QWidget *TelaInicial)
    {
        TelaInicial->setWindowTitle(QCoreApplication::translate("TelaInicial", "Truco Paulista - Tela Inicial", nullptr));
        botaoLogin->setText(QCoreApplication::translate("TelaInicial", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TelaInicial: public Ui_TelaInicial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELAINICIAL_H
