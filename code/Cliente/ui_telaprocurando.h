/********************************************************************************
** Form generated from reading UI file 'telaprocurando.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELAPROCURANDO_H
#define UI_TELAPROCURANDO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelaProcurando
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblStatus;
    QSpacerItem *spacer;
    QPushButton *btnCancelarProcura;

    void setupUi(QWidget *TelaProcurando)
    {
        if (TelaProcurando->objectName().isEmpty())
            TelaProcurando->setObjectName("TelaProcurando");
        TelaProcurando->resize(400, 300);
        verticalLayout = new QVBoxLayout(TelaProcurando);
        verticalLayout->setObjectName("verticalLayout");
        lblStatus = new QLabel(TelaProcurando);
        lblStatus->setObjectName("lblStatus");
        lblStatus->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblStatus);

        spacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(spacer);

        btnCancelarProcura = new QPushButton(TelaProcurando);
        btnCancelarProcura->setObjectName("btnCancelarProcura");

        verticalLayout->addWidget(btnCancelarProcura);


        retranslateUi(TelaProcurando);

        QMetaObject::connectSlotsByName(TelaProcurando);
    } // setupUi

    void retranslateUi(QWidget *TelaProcurando)
    {
        TelaProcurando->setWindowTitle(QCoreApplication::translate("TelaProcurando", "Procurando Partida", nullptr));
        lblStatus->setText(QCoreApplication::translate("TelaProcurando", "Procurando jogadores...", nullptr));
        btnCancelarProcura->setText(QCoreApplication::translate("TelaProcurando", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TelaProcurando: public Ui_TelaProcurando {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELAPROCURANDO_H
