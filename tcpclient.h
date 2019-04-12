#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include "baseparent.h"

class TcpClient : public BaseParent
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

public:
    bool Open(Para *para);
    void Close();
    void handleReadyRead();
    void slot_send_data(QByteArray hex);
private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
