#include "serialport.h"
#include <QIODevice>

SerialPort::SerialPort(QObject *parent) : BaseParent(parent)
{
    serial = new QSerialPort(this);
    connect(serial,&QSerialPort::readyRead,this,&SerialPort::handleReadyRead);
}

bool SerialPort::Open(Para *para)
{
    if(serial->isOpen())
        serial->close();
    serial->setPortName(QString("COM%1").arg(para->com));
    if(!serial->open(QIODevice::ReadWrite))
        return false;
    serial->setBaudRate(para->baud);
    serial->setParity(para->parity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    return true;
}

void SerialPort::Close()
{
    serial->close();
}

void SerialPort::handleReadyRead()
{
    emit sig_recv_data(serial->readAll());
}

void SerialPort::slot_send_data(QByteArray hex)
{
    if(serial->isOpen() && serial->isWritable()) {
        serial->write(hex);
        serial->flush();
    }
}
