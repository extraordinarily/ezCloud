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
    socket.setSocketOption(QAbstractSocket::KeepAliveOption,true);
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
    flag = 0;
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
        int hi = (unsigned int)(unsigned char) mBuf[0];
        int lo = (unsigned int)(unsigned char) mBuf[1];
        hi = hi*256+lo;
        if (len >= hi+4)
        {
            if (mBuf[2]==8)
            {
                int num = hi/256;
                int index = 0;
                int tmp = 4;
                for (int i=0;i<num;i++)
                {
                    // 192 filename
                    // 50  MD5
                    // 1   cookie
                    // 1   flag
                    // 4   ip
                    // 4   MB
                    // 4   BB
                    std::string filename(mBuf+tmp,mBuf+tmp+strlen(mBuf+tmp));
                    std::string md5(mBuf+tmp+192,mBuf+tmp+192+strlen(mBuf+tmp+192));
                    int cookie = mBuf[tmp+192+50];
                    int flag = mBuf[tmp+192+51];
                    quint32 ip = *(int *)(mBuf+tmp+244);
                    long long MB = *(int *)(mBuf+tmp+248);
                    long long BB = *(int *)(mBuf+tmp+252);
                    BB += MB<<20;
                    QHostAddress aaa(ip);
                    item[index++] = QString::fromStdString(filename);
                    item[index++] = QString::fromStdString(md5);
                    item[index++] = QString::number(cookie);
                    item[index++] = aaa.toString();
                    item[index++] = QString::number(BB);
                    item[index++] = flag ? "在线" : "不在";
                    tmp+=256;
                }
                emit refreshOver(num);
            }
            for (int i=0;i<len-hi-4;i++) mBuf[i] = mBuf[i+4+hi];
            len -= (hi+4);
        }
    }
    flag = 0;
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
    qDebug()<<flag;
    if (flag==0)
    {
        flag=1;
        return ;
    }

    if (flag==1)
    {
        char buf[4]; buf[0]=0; buf[1]=0; buf[2]=0; buf[3]=0;
        flag=2;
        socket.write(buf,4);
        if (!socket.waitForBytesWritten())
        {
            errorHandler();
            return ;
        }
        socket.flush();
        return ;
    }

    if (flag==2)
    {
        errorHandler();
        return ;
    }
}

void ControlSocket::sendMSG(QByteArray msg)
{
    socket.write(msg);
    socket.waitForBytesWritten();
    socket.flush();
}
