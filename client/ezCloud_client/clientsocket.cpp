#include "clientsocket.h"
#include<QDebug>
ClientSocket::ClientSocket(QObject *parent) : QObject(parent)
{
    state = 0;
}

void ClientSocket::login()
{
    if (state!=0) return ;
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
        //send why be why be
        // {your user name} or {so}
        if (usernameLineEdit->text()!="")
        {
            char tmp[100];
            strcpy(tmp,usernameLineEdit->text().toStdString().c_str());
            socket.write(tmp,strlen(tmp));
            if (socket.waitForBytesWritten())
            {
                //ok
                if (socket.waitForReadyRead())
                {
                    //设置好，做好信号槽
                    state = 1;
                }
                else error(4);
            }
            else error(4);
            //发送失败
        }
        else
        {
            emit error(3);
            //无用户名
        }
    }
    else
    {
        emit error(2);
        // 连接失败
    }
}

void ClientSocket::moveToThreadAll(QThread *thread)
{
    socket.moveToThread(thread);
    this->moveToThread(thread);
}
