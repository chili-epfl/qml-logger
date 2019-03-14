#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:
    void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};

#endif // SERVER_H
