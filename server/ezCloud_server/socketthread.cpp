#include "socketthread.h"
#include <QDebug>
SocketThread::SocketThread(qintptr handle)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    moveToThread(this);
    socket->moveToThread(this);
}

void SocketThread::run()
{
    if (socket->waitForConnected())
    {
        qDebug()<<"connected";
        connect(socket,&QTcpSocket::readyRead,this,&SocketThread::messageHandler,Qt::QueuedConnection);
    }
    else
    {
        qDebug()<<"error";
    }
    exec();
}

void SocketThread::messageHandler()
{
    // change to switch case
    char tmp[100];
    memcpy(tmp,socket->readAll(),20);
    if (tmp[2]==0)
    {
        tmp[2] = 0;
        tmp[1] = 0;
        socket->write(tmp,4);
        socket->waitForBytesWritten();
        return ;
    }
    else if (tmp[2]==1)
    {
        tmp[2] = 3;
        tmp[1] = 0;
        socket->write(tmp,4);
        socket->waitForBytesWritten();
        return ;
    }
    // messageHandler
}
