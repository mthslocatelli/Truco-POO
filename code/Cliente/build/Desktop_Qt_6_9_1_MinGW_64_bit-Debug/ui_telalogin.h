/********************************************************************************
** Form generated from reading UI file 'telalogin.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELALOGIN_H
#define UI_TELALOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelaLogin
{
public:
    QLineEdit *campoNome;
    QPushButton *botaoEntrar;
    QLineEdit *campoSenha;
    QLabel *label;

    void setupUi(QWidget *TelaLogin)
    {
        if (TelaLogin->objectName().isEmpty())
            TelaLogin->setObjectName("TelaLogin");
        TelaLogin->resize(474, 343);
        campoNome = new QLineEdit(TelaLogin);
        campoNome->setObjectName("campoNome");
        campoNome->setGeometry(QRect(140, 60, 200, 30));
        botaoEntrar = new QPushButton(TelaLogin);
        botaoEntrar->setObjectName("botaoEntrar");
        botaoEntrar->setGeometry(QRect(190, 140, 100, 40));
        campoSenha = new QLineEdit(TelaLogin);
        campoSenha->setObjectName("campoSenha");
        campoSenha->setGeometry(QRect(140, 100, 200, 30));
        label = new QLabel(TelaLogin);
        label->setObjectName("label");
        label->setGeometry(QRect(200, 190, 91, 21));
        label->setTextFormat(Qt::TextFormat::AutoText);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        retranslateUi(TelaLogin);

        QMetaObject::connectSlotsByName(TelaLogin);
    } // setupUi

    void retranslateUi(QWidget *TelaLogin)
    {
        TelaLogin->setWindowTitle(QCoreApplication::translate("TelaLogin", "Login - Truco Paulista", nullptr));
        campoNome->setPlaceholderText(QCoreApplication::translate("TelaLogin", "Digite seu nome...", nullptr));
        botaoEntrar->setText(QCoreApplication::translate("TelaLogin", "Entrar", nullptr));
        campoSenha->setPlaceholderText(QCoreApplication::translate("TelaLogin", "Digite seu nome...", nullptr));
        label->setText(QCoreApplication::translate("TelaLogin", "Criar conta", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TelaLogin: public Ui_TelaLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELALOGIN_H
