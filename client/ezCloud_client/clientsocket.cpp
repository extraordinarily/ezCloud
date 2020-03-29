#include "clientsocket.h"
#include<QDebug>
ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{

}

void ClientSocket::login()
{
    qDebug()<<socket.thread();
    qDebug()<<this->thread();
    QHostAddress serverIP;
    if (!serverIP.setAddress(ipLineEdit->text()))
    {
        emit error(1);
        return ;
    }
    socket.connectToHost(serverIP,30142);
    if (socket.waitForConnected())
    {
        qDebug()<<"connected!";
    }
    else
    {
        emit error(2);
    }
}

void ClientSocket::moveToThreadAll(QThread *thread)
{
    socket.moveToThread(thread);
    this->moveToThread(thread);
}
