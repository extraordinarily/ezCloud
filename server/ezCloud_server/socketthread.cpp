#include "socketthread.h"
#include <QDebug>
SocketThread::SocketThread(qintptr handle)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    moveToThread(this);
    socket->moveToThread(this);
    len = 0;
}

void SocketThread::run()
{
    if (socket->waitForConnected())
    {
        connect(socket,&QTcpSocket::readyRead,this,&SocketThread::messageHandler,Qt::QueuedConnection);
        connect(socket,&QTcpSocket::disconnected,this,&SocketThread::error,Qt::QueuedConnection);
        ip = socket->peerAddress().toIPv4Address();
        socket->setSocketOption(QAbstractSocket::KeepAliveOption,true);
    }
    else
    {
        socket->close();
        error();
        return ;
    }
    exec();
}

void SocketThread::messageHandler()
{
    QByteArray buffer = socket->readAll();
    int buflen = buffer.length();
    memcpy(buf+len,buffer,buflen);
    len += buflen;
    if (len < 4) return ;
    int hi = (unsigned int)(unsigned char) buf[0];
    int lo = (unsigned int)(unsigned char) buf[1];
    hi = hi*256+lo;
    if (len < hi+4) return;
    if (buf[2]==0)
    {
        for (int i=0;i<len-hi-4;i++) buf[i] = buf[i+4+hi];
        len -= (hi+4);
        char tmp[4];
        tmp[0]=tmp[1]=tmp[2]=tmp[3]=0;
        socket->write(tmp,4);
        socket->waitForBytesWritten();
        socket->flush();
        return ;
    }
    QByteArray packet(buf,hi+4);
    for (int i=0;i<len-hi-4;i++) buf[i] = buf[i+4+hi];
    len -= (hi+4);
    emit dbHandler(this,packet);
}

void SocketThread::sendMessage(QByteArray message)
{
    socket->write(message);
    socket->waitForBytesWritten();
    socket->flush();
}

void SocketThread::error()
{
    emit delMe(this);
}
