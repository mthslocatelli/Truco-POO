#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <QTcpSocket> // Usando QTcpSocket
#include <QHostAddress> // Para endereços IP
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream> // Para ler/escrever o tamanho da mensagem
#include <QAbstractSocket> // Para tratamento de erros de socket

class ClienteTruco : public QObject
{
    Q_OBJECT
public:
    explicit ClienteTruco(QObject *parent = nullptr);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QJsonObject &message);
    void error(const QString &errorString);

public slots:
    void connectToServer(const QString &address, quint16 port);
    void disconnectFromServer();
    void sendCommand(const QJsonObject &command);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead(); // Slot para ler dados do socket
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_tcpSocket; // Objeto QTcpSocket
    QByteArray m_buffer;      // Buffer para dados recebidos
    quint32 m_nextBlockSize; // Tamanho do próximo bloco a ser lido
};

#endif // CLIENTETRUCO_H
