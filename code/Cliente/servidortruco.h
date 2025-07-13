#ifndef SERVIDORTRUCO_H
#define SERVIDORTRUCO_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class ServidorTruco : public QTcpServer {
    Q_OBJECT
public:
    explicit ServidorTruco(QObject *parent = nullptr);
    void startServer(quint16 porta);

signals:
    void novaMensagem(QString mensagem);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void lerDados();
    void clienteDesconectado();

private:
    QList<QTcpSocket*> clientes;
};

#endif // SERVIDORTRUCO_H
#ifndef SERVIDORTRUCO_H
#define SERVIDORTRUCO_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class ServidorTruco : public QTcpServer {
    Q_OBJECT
public:
    explicit ServidorTruco(QObject *parent = nullptr);
    void startServer(quint16 porta);

signals:
    void novaMensagem(QString mensagem);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void lerDados();
    void clienteDesconectado();

private:
    QList<QTcpSocket*> clientes;
};

#endif // SERVIDORTRUCO_H
