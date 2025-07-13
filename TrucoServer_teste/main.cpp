#include <QCoreApplication>
#include "echoserver.h"
#include "gameengine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameEngine* game = GameEngine::getInstance();

    // O servidor escutará na porta 5000
    EchoServer server(game, 5000);

    return a.exec();
}
