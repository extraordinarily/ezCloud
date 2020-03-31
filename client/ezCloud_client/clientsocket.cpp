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
    // 启动心跳机制
    // 启动端口监听
    qDebug()<<cookie;
}

void ClientSocket::moveToThreadAll(QThread *thread)
{
    socket.moveToThread(thread);
    this->moveToThread(thread);
}
