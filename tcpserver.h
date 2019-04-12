#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "baseparent.h"

class TcpServer : public BaseParent
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
public:
    bool Open(Para *para);
    void Close();
    void slot_send_data(QByteArray hex);
private:
    void handleReadyRead();
    void handleNewConnection();
private:
    QTcpServer *server;
    QTcpSocket *socket = NULL;
};

#endif // TCPSERVER_H
