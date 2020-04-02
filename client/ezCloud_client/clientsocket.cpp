#include "clientsocket.h"
#include<QDebug>
ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{
    state = 0;
    cookie = 123;
}

void ClientSocket::login()
{
    if (state!=0) return ;
    QHostAddress serverIP;
    if (!serverIP.setAddress(ipLineEdit->text()))
    {
        // check ip
        emit error(1);
        socket.abort();
        return ;
    }
    if (usernameLineEdit->text()=="")
    {
        // need username
        emit error(2);
        socket.abort();
        return ;
    }
    socket.connectToHost(serverIP,30142);
    if (!socket.waitForConnected())
    {
        // check network
        emit error(3);
        socket.abort();
        return ;
    }

    // generate the login packet
    char tmp[64];
    tmp[0]=0; tmp[1]=16; tmp[2]=1; tmp[3]=cookie;
    strcpy(tmp+4,usernameLineEdit->text().toStdString().c_str());
    for (int i=strlen(tmp+4)+4;i<12;i++) tmp[i]=32;
    strcpy(tmp+12,passwordLineEdit->text().toStdString().c_str());
    for (int i=strlen(tmp+12)+12;i<20;i++) tmp[i]=32;
    socket.write(tmp,20);

    if (!socket.waitForBytesWritten())
    {
        // check network
        emit error(3);
        socket.abort();
        return ;
    }
    if (!socket.waitForReadyRead())
    {
        // check network
        emit error(3);
        socket.abort();
        return ;
    }
    strcpy(tmp,socket.readAll().toStdString().c_str());
    if (tmp[2]==2)
    {
        // login error
        emit error(4);
        socket.abort();
        return ;
    }
    // login success
    state = 1;
    cookie = tmp[3];
    connect(&socket,&QTcpSocket::readyRead,this,&ClientSocket::messageHandler,Qt::QueuedConnection);
    connect(&timer,&QTimer::timeout,this,&ClientSocket::heatBeatHandler,Qt::QueuedConnection);
    emit changeToDownload();
    active = std::chrono::steady_clock::now();
    timer.start(10000);
}

void ClientSocket::logout()
{
    timer.stop();
    socket.disconnectFromHost();
    state = 0;
}

void ClientSocket::messageHandler()
{
    // fill buffer
    // check buffer
    char tmp[100];
    memcpy(tmp,socket.readAll(),4);
    active = std::chrono::steady_clock::now();
    if (tmp[2]==0)
    {

    }
    {
        // emit showRefresh();
    }
}

void ClientSocket::heatBeatHandler()
{
    if (state==0) return ;
    auto gap = std::chrono::steady_clock::now() - active;
    if (gap > std::chrono::seconds(20))
    {
        logout();
        emit error(4);
    }
    else if (gap > std::chrono::seconds(10))
    {
        char tmp[64];
        tmp[0]=0; tmp[1]=0; tmp[2]=0; tmp[3]=cookie;
        socket.write(tmp,4);
        if (!socket.waitForBytesWritten())
        {
            logout();
            emit error(4);
        }
    }
}

void ClientSocket::refresh()
{
    char tmp[64];
    tmp[0]=0; tmp[1]=0; tmp[2]=3; tmp[3]=cookie;
    socket.write(tmp,4);
    if (!socket.waitForBytesWritten())
    {
        logout();
        emit error(4);
    }
}

void ClientSocket::moveToThreadAll(QThread *thread)
{
    socket.moveToThread(thread);
    timer.moveToThread(thread);
    this->moveToThread(thread);
}

