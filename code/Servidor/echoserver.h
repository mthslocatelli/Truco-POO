#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMap>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include <QCoreApplication>
#include "gameengine.h"

class EchoServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoServer(GameEngine* gameEngine, quint16 port, QObject *parent = nullptr);
    ~EchoServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    void sendResponse(QTcpSocket *client, const QJsonObject &message);
    void processCommand(QTcpSocket *client, const QJsonObject &command);
    void broadcastToSala(const QString& codigoSala, const QJsonObject& message);
    void processarProximoTurnoBot(const QString& codigoSala, int idxBotAtual);

    GameEngine* game;
    QTcpServer *m_tcpServer;
    // Buffer para dados incompletos e tamanho do próximo bloco para cada cliente
    QMap<QTcpSocket*, QByteArray> m_clientBuffers;
    QMap<QTcpSocket*, quint32> m_clientNextBlockSize;
    QMap<QString, QList<QTcpSocket*>> clientesPorSala;
};

#endif // ECHOSERVER_H
