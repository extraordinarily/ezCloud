#include "socketthread.h"
#include <QDebug>
SocketThread::SocketThread(qintptr handle)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    moveToThread(this);
    socket->moveToThread(this);
    connect(socket,&QTcpSocket::readyRead,this,&SocketThread::messageHandler,Qt::QueuedConnection);
}

void SocketThread::run()
{
    if (socket->waitForConnected())
    {
        qDebug()<<"connected";
    }
    else
    {
        qDebug()<<"error";
    }
    exec();
}

void SocketThread::messageHandler()
{
    QString ss=QVariant(socket->readAll()).toString();
    qDebug()<<ss;
}
