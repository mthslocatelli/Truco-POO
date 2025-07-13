/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnCarta1;
    QPushButton *btnCarta2;
    QPushButton *btnCarta3;
    QPushButton *btnTruco;
    QLabel *lblManilha;
    QLabel *lblPontos;
    QLabel *lblTruco;
    QLabel *lblMesa;
    QLabel *pontoNos1;
    QLabel *pontoNos2;
    QLabel *pontoNos3;
    QLabel *mesa;
    QLabel *pontoEles1;
    QLabel *pontoEles3;
    QLabel *pontoEles2;
    QLabel *cartaJogador1_1;
    QLabel *cartaJogador1_2;
    QLabel *cartaJogador1_3;
    QLabel *cartaJogador2_1;
    QLabel *cartaJogador2_2;
    QLabel *cartaJogador2_3;
    QLabel *cartaJogador3_1;
    QLabel *cartaJogador3_2;
    QLabel *cartaJogador3_3;
    QPushButton *criarSala;
    QPushButton *entrarSala;
    QLabel *chatBox;
    QLineEdit *entradaCodigo;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: #2F434F;"));
        btnCarta1 = new QPushButton(centralwidget);
        btnCarta1->setObjectName("btnCarta1");
        btnCarta1->setGeometry(QRect(300, 500, 120, 190));
        btnCarta1->setIconSize(QSize(300, 300));
        btnCarta2 = new QPushButton(centralwidget);
        btnCarta2->setObjectName("btnCarta2");
        btnCarta2->setGeometry(QRect(460, 500, 120, 190));
        btnCarta2->setIconSize(QSize(300, 300));
        btnCarta3 = new QPushButton(centralwidget);
        btnCarta3->setObjectName("btnCarta3");
        btnCarta3->setGeometry(QRect(620, 500, 120, 190));
        btnCarta3->setIconSize(QSize(300, 300));
        btnTruco = new QPushButton(centralwidget);
        btnTruco->setObjectName("btnTruco");
        btnTruco->setGeometry(QRect(410, 310, 93, 29));
        lblManilha = new QLabel(centralwidget);
        lblManilha->setObjectName("lblManilha");
        lblManilha->setGeometry(QRect(600, 200, 90, 150));
        lblManilha->setTextFormat(Qt::TextFormat::AutoText);
        lblManilha->setScaledContents(true);
        lblPontos = new QLabel(centralwidget);
        lblPontos->setObjectName("lblPontos");
        lblPontos->setGeometry(QRect(430, 170, 47, 20));
        lblTruco = new QLabel(centralwidget);
        lblTruco->setObjectName("lblTruco");
        lblTruco->setGeometry(QRect(400, 240, 112, 20));
        lblMesa = new QLabel(centralwidget);
        lblMesa->setObjectName("lblMesa");
        lblMesa->setGeometry(QRect(300, 300, 400, 171));
        lblMesa->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lblMesa->setFrameShape(QFrame::Shape::Box);
        lblMesa->setAlignment(Qt::AlignmentFlag::AlignTop);
        pontoNos1 = new QLabel(centralwidget);
        pontoNos1->setObjectName("pontoNos1");
        pontoNos1->setGeometry(QRect(560, 250, 20, 20));
        pontoNos1->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoNos1->setScaledContents(true);
        pontoNos2 = new QLabel(centralwidget);
        pontoNos2->setObjectName("pontoNos2");
        pontoNos2->setGeometry(QRect(590, 250, 20, 20));
        pontoNos2->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoNos2->setScaledContents(true);
        pontoNos3 = new QLabel(centralwidget);
        pontoNos3->setObjectName("pontoNos3");
        pontoNos3->setGeometry(QRect(620, 250, 20, 20));
        pontoNos3->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoNos3->setScaledContents(true);
        mesa = new QLabel(centralwidget);
        mesa->setObjectName("mesa");
        mesa->setGeometry(QRect(0, 0, 1000, 800));
        mesa->setPixmap(QPixmap(QString::fromUtf8("mesa.png")));
        mesa->setScaledContents(true);
        pontoEles1 = new QLabel(centralwidget);
        pontoEles1->setObjectName("pontoEles1");
        pontoEles1->setGeometry(QRect(680, 250, 20, 20));
        pontoEles1->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoEles1->setScaledContents(true);
        pontoEles3 = new QLabel(centralwidget);
        pontoEles3->setObjectName("pontoEles3");
        pontoEles3->setGeometry(QRect(740, 250, 20, 20));
        pontoEles3->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoEles3->setScaledContents(true);
        pontoEles2 = new QLabel(centralwidget);
        pontoEles2->setObjectName("pontoEles2");
        pontoEles2->setGeometry(QRect(710, 250, 20, 20));
        pontoEles2->setPixmap(QPixmap(QString::fromUtf8("imagens/circulo_verde.png")));
        pontoEles2->setScaledContents(true);
        cartaJogador1_1 = new QLabel(centralwidget);
        cartaJogador1_1->setObjectName("cartaJogador1_1");
        cartaJogador1_1->setGeometry(QRect(90, 350, 63, 20));
        cartaJogador1_2 = new QLabel(centralwidget);
        cartaJogador1_2->setObjectName("cartaJogador1_2");
        cartaJogador1_2->setGeometry(QRect(90, 380, 63, 20));
        cartaJogador1_3 = new QLabel(centralwidget);
        cartaJogador1_3->setObjectName("cartaJogador1_3");
        cartaJogador1_3->setGeometry(QRect(90, 410, 63, 20));
        cartaJogador2_1 = new QLabel(centralwidget);
        cartaJogador2_1->setObjectName("cartaJogador2_1");
        cartaJogador2_1->setGeometry(QRect(420, 140, 63, 20));
        cartaJogador2_2 = new QLabel(centralwidget);
        cartaJogador2_2->setObjectName("cartaJogador2_2");
        cartaJogador2_2->setGeometry(QRect(420, 110, 63, 20));
        cartaJogador2_3 = new QLabel(centralwidget);
        cartaJogador2_3->setObjectName("cartaJogador2_3");
        cartaJogador2_3->setGeometry(QRect(420, 80, 63, 20));
        cartaJogador3_1 = new QLabel(centralwidget);
        cartaJogador3_1->setObjectName("cartaJogador3_1");
        cartaJogador3_1->setGeometry(QRect(850, 400, 63, 20));
        cartaJogador3_2 = new QLabel(centralwidget);
        cartaJogador3_2->setObjectName("cartaJogador3_2");
        cartaJogador3_2->setGeometry(QRect(850, 370, 63, 20));
        cartaJogador3_3 = new QLabel(centralwidget);
        cartaJogador3_3->setObjectName("cartaJogador3_3");
        cartaJogador3_3->setGeometry(QRect(850, 340, 63, 20));
        criarSala = new QPushButton(centralwidget);
        criarSala->setObjectName("criarSala");
        criarSala->setGeometry(QRect(90, 120, 93, 29));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush);
        criarSala->setPalette(palette);
        criarSala->setAutoFillBackground(false);
        criarSala->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        entrarSala = new QPushButton(centralwidget);
        entrarSala->setObjectName("entrarSala");
        entrarSala->setGeometry(QRect(90, 170, 93, 29));
        QPalette palette1;
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush);
        entrarSala->setPalette(palette1);
        entrarSala->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        chatBox = new QLabel(centralwidget);
        chatBox->setObjectName("chatBox");
        chatBox->setGeometry(QRect(190, 120, 150, 26));
        QPalette palette2;
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush);
        chatBox->setPalette(palette2);
        chatBox->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        entradaCodigo = new QLineEdit(centralwidget);
        entradaCodigo->setObjectName("entradaCodigo");
        entradaCodigo->setGeometry(QRect(190, 170, 100, 26));
        entradaCodigo->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        MainWindow->setCentralWidget(centralwidget);
        mesa->raise();
        btnCarta1->raise();
        btnCarta2->raise();
        btnCarta3->raise();
        btnTruco->raise();
        lblManilha->raise();
        lblPontos->raise();
        lblTruco->raise();
        lblMesa->raise();
        pontoNos1->raise();
        pontoNos2->raise();
        pontoNos3->raise();
        pontoEles1->raise();
        pontoEles3->raise();
        pontoEles2->raise();
        cartaJogador1_1->raise();
        cartaJogador1_2->raise();
        cartaJogador1_3->raise();
        cartaJogador2_1->raise();
        cartaJogador2_2->raise();
        cartaJogador2_3->raise();
        cartaJogador3_1->raise();
        cartaJogador3_2->raise();
        cartaJogador3_3->raise();
        criarSala->raise();
        entrarSala->raise();
        chatBox->raise();
        entradaCodigo->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Truco Paulista", nullptr));
        btnCarta1->setText(QString());
        btnCarta2->setText(QString());
        btnCarta3->setText(QString());
        btnTruco->setText(QCoreApplication::translate("MainWindow", "Pedir Truco", nullptr));
        lblManilha->setText(QCoreApplication::translate("MainWindow", "Manilha:", nullptr));
        lblPontos->setText(QCoreApplication::translate("MainWindow", "Pontos:", nullptr));
        lblTruco->setText(QCoreApplication::translate("MainWindow", "Rodada valendo:", nullptr));
        lblMesa->setText(QCoreApplication::translate("MainWindow", "Cartas na mesa:", nullptr));
        pontoNos1->setText(QString());
        pontoNos2->setText(QString());
        pontoNos3->setText(QString());
        mesa->setText(QString());
        pontoEles1->setText(QString());
        pontoEles3->setText(QString());
        pontoEles2->setText(QString());
        cartaJogador1_1->setText(QString());
        cartaJogador1_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador1_3->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador2_1->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador2_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador2_3->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador3_1->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador3_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        cartaJogador3_3->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        criarSala->setText(QCoreApplication::translate("MainWindow", "Criar Sala", nullptr));
        entrarSala->setText(QCoreApplication::translate("MainWindow", "Entrar Sala", nullptr));
        chatBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
