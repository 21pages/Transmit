#include "tcpclient.h"
#include <QCoreApplication>
#include <QTime>

TcpClient::TcpClient(QObject *parent) : BaseParent(parent)
{
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&TcpClient::handleReadyRead);
}

bool TcpClient::Open(Para *para)
{
    socket->connectToHost(para->ip, para->client_port);
    QTime time;
    time.start();
    while((time.elapsed() < 10000) && (socket->state()!=QAbstractSocket::ConnectedState)) {
        QCoreApplication::processEvents();
    }
    return (socket->state()==QAbstractSocket::ConnectedState);
}

void TcpClient::Close()
{
    socket->disconnectFromHost();
}

void TcpClient::handleReadyRead()
{
    emit sig_recv_data(socket->readAll());
}

void TcpClient::slot_send_data(QByteArray hex)
{
    if(socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(hex);
        socket->flush();
    }
}
