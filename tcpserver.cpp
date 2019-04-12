#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : BaseParent(parent)
{
    server = new QTcpServer(this);
    connect(server,&QTcpServer::newConnection,this,&TcpServer::handleNewConnection);
}

bool TcpServer::Open(Para *para)
{
    return server->listen(QHostAddress::AnyIPv4,para->server_port);
}

void TcpServer::Close()
{
    if(socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = NULL;
    }
    server->close();
}

//void TcpServer::incomingConnection(qintptr handle)
//{
//    if (!socket) {
//        socket = new QTcpSocket(this);
//    } else {
//        disconnect(socket,&QTcpSocket::readyRead,this,&TcpServer::handleReadyRead);
//        socket->disconnectFromHost();
//    }
//    socket->setSocketDescriptor(handle);
//    connect(socket,&QTcpSocket::readyRead,this,&TcpServer::handleReadyRead);
//}

void TcpServer::handleReadyRead()
{
    emit sig_recv_data(socket->readAll());
}

void TcpServer::handleNewConnection()
{
    socket = server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&TcpServer::handleReadyRead);
}

void TcpServer::slot_send_data(QByteArray hex)
{
    if(socket && socket->isWritable()) {
        socket->write(hex);
        socket->flush();
    }
}
