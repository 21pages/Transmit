#ifndef BASEPARENT_H
#define BASEPARENT_H

#include <QObject>

#include <QSerialPort>

struct Para
{
    int com;
    int baud;
    QSerialPort::Parity parity;

    quint16 server_port;

    QString ip;
    quint16 client_port;
};

class BaseParent : public QObject
{
    Q_OBJECT
public:
    explicit BaseParent(QObject *parent = nullptr);
public:
    virtual bool Open(Para *para) = 0;
    virtual void Close() = 0;
    virtual void slot_send_data(QByteArray hex) = 0;
signals:
    void sig_recv_data(QByteArray hex);
};

#endif // BASEPARENT_H
