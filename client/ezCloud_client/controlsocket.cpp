#include "controlsocket.h"
#include <QDebug>
ControlSocket::ControlSocket()
{
    moveToThread(this);
    socket.moveToThread(this);
    timer.moveToThread(this);
    len = 0;
    start();
}

ControlSocket::~ControlSocket()
{
}

void ControlSocket::login(QString ip,int port)
{
    socket.connectToHost(ip,port);
    if (!socket.waitForConnected())
    {
        socket.close();
        emit loginOver(false);
        return ;
    }
    char buf[4]; buf[0]=0; buf[1]=0; buf[2]=4; buf[3]=cookie;
    socket.write(buf,4);
    if (!socket.waitForBytesWritten())
    {
        socket.close();
        emit loginOver(false);
        return ;
    }
    socket.flush();
    if (!socket.waitForReadyRead())
    {
        qDebug()<<"ha";
        socket.close();
        emit loginOver(false);
        return ;
    }
    memcpy(buf,socket.readAll(),4);
    if (buf[2]!=5)
    {
        socket.close();
        emit loginOver(false);
        return ;
    }
    connect(&socket,&QTcpSocket::readyRead,this,&ControlSocket::messageHandler,Qt::QueuedConnection);
    connect(&timer,&QTimer::timeout,this,&ControlSocket::heartBeat,Qt::QueuedConnection);
    connect(&socket,&QTcpSocket::disconnected,this,&ControlSocket::errorHandler);
    emit loginOver(true);
    active = std::chrono::steady_clock::now();
    timer.start(5000);
}

void ControlSocket::stopAll()
{
    disconnect(&socket,&QTcpSocket::disconnected,this,&ControlSocket::errorHandler);
    socket.close();
    timer.stop();
}

void ControlSocket::errorHandler()
{
    stopAll();
    emit error();
}

void ControlSocket::messageHandler()
{
    QByteArray tmp = socket.readAll();
    int tmpLen = tmp.length();
    memcpy(mBuf+len,tmp,tmpLen);
    len+=tmpLen;
    if (len>=4)
    {
        int hi = (unsigned int) mBuf[0];
        int lo = (unsigned int) mBuf[1];
        hi = hi*256+lo;
        if (len >= hi)
        {
            if (mBuf[2]==8)
            {
                //chuli

                // filename MD5 cookie ip length zaixian
                item[0] = "fq.avi";
                item[1] = "hhhhxxaslk";
                item[2] = "jiushini";
                item[3] = "192.168.1.3";
                item[4] = "50K";
                item[5] = "zai";

                item[0+6] = "fq.avi";
                item[1+6] = "hhhhxxaslk";
                item[2+6] = "jiushini";
                item[3+6] = "192.168.1.3";
                item[4+6] = "50K";
                item[5+6] = "zai";

                emit refreshOver(2);
            }
            for (int i=0;i<len-hi-4;i++) mBuf[i] = mBuf[i+4+hi];
            len -= (hi+4);
        }
    }
    active = std::chrono::steady_clock::now();
}

void ControlSocket::refresh()
{
    char buf[4]; buf[0]=0; buf[1]=0; buf[2]=7; buf[3]=cookie;
    socket.write(buf,4);
    if (!socket.waitForBytesWritten())
    {
        errorHandler();
        return ;
    }
    socket.flush();
}


void ControlSocket::heartBeat()
{
    auto gap = std::chrono::steady_clock::now() - active;
    if (gap > std::chrono::seconds(10))
    {
        errorHandler();
        return ;
    }
    if (gap > std::chrono::seconds(5))
    {
        char buf[4]; buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        socket.write(buf,4);
        if (!socket.waitForBytesWritten())
        {
            errorHandler();
            return ;
        }
        socket.flush();
    }
}
