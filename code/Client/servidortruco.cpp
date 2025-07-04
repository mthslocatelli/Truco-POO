#include "servidortruco.h"
#include <QTextStream>

ServidorTruco::ServidorTruco(QObject *parent) : QTcpServer(parent) {}

void ServidorTruco::startServer(quint16 porta) {
    if (this->listen(QHostAddress::Any, porta)) {
        qDebug() << "Servidor iniciado na porta" << porta;
    } else {
        qDebug() << "Erro ao iniciar servidor:" << this->errorString();
    }
}

void ServidorTruco::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *cliente = new QTcpSocket(this);
    cliente->setSocketDescriptor(socketDescriptor);
    connect(cliente, &QTcpSocket::readyRead, this, &ServidorTruco::lerDados);
    connect(cliente, &QTcpSocket::disconnected, this, &ServidorTruco::clienteDesconectado);
    clientes.append(cliente);
    qDebug() << "Cliente conectado:" << cliente->peerAddress().toString();
}

void ServidorTruco::lerDados() {
    QTcpSocket *cliente = qobject_cast<QTcpSocket*>(sender());
    if (!cliente) return;

    while (cliente->canReadLine()) {
        QString msg = QString::fromUtf8(cliente->readLine()).trimmed();
        emit novaMensagem(msg);

        // Envia para todos os clientes (broadcast)
        for (QTcpSocket *outro : clientes) {
            if (outro != cliente)
                outro->write((msg + "\n").toUtf8());
        }
    }
}

void ServidorTruco::clienteDesconectado() {
    QTcpSocket *cliente = qobject_cast<QTcpSocket*>(sender());
    if (!cliente) return;

    clientes.removeAll(cliente);
    cliente->deleteLater();
    qDebug() << "Cliente desconectado.";
}
