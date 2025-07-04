QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = truco_paulista
TEMPLATE = app

RESOURCES += resources.qrc

SOURCES += main.cpp \
           clientetruco.cpp \
           jogadorhumano.cpp \
           mainwindow.cpp \
           gameengine.cpp \
           carta.cpp \
           baralho.cpp \
           jogador.cpp \
           jogadorbot.cpp

HEADERS  += mainwindow.h \
            clientetruco.h \
            gameengine.h \
            carta.h \
            baralho.h \
            jogador.h \
            jogadorbot.h \
            jogadorhumano.h

FORMS    += mainwindow.ui
