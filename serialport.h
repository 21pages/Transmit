#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include "baseparent.h"

class SerialPort : public BaseParent
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
public:
    bool Open(Para *para);
    void Close();
    void handleReadyRead();
    void slot_send_data(QByteArray hex);
private:
    QSerialPort *serial;
};

#endif // SERIALPORT_H
