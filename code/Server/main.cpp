#include <QCoreApplication>
#include "echoserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // O servidor escutará na porta 5000
    EchoServer server(5000);

    return a.exec();
}
