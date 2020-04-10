#include "ezserver.h"
#include <QDebug>

EZServer::EZServer(QWidget *win)
{
    mainwin = win;
    db.close();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("UserData");
    db.open();
    QSqlQuery query;
    query.exec("create table if not exists tcookie (cookie integer primary key autoincrement,flag integer)");
    query.exec("create table if not exists tfile (cookie integer primary key autoincrement,flag integer)");
}

EZServer::~EZServer()
{
    for (std::list<SocketThread *>::iterator
         it = socketList.begin();it!=socketList.end();it++)
    {
        (*it)->quit();
        (*it)->wait();
        delete (*it);
    }
    db.close();
}

void EZServer::incomingConnection(qintptr handle)
{
    SocketThread * thread = new SocketThread(handle);
    // 关闭的话就清除socket 做个时钟定时器 emit
    connect(thread,&SocketThread::dbHandler,this,&EZServer::dbHandler);
    thread->start();
    socketList.push_back(thread);
}

void EZServer::dbHandler(SocketThread * thread,QByteArray packet)
{
    QByteArray message;
    QSqlQuery query;
    if (packet.data()[2]==1)
    {
        char tmp[4];
        if (QMessageBox::question(mainwin, "新的设备", "想要分发cookie吗?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            query.exec("insert into tcookie (flag) values (0)");
            query.exec("select max(cookie) from tcookie");
            query.next();
            int num = query.value(0).toInt();
            tmp[0]=0; tmp[1]=0; tmp[2]=2; tmp[3]=num;
        }
        else
        {
            tmp[0]=0; tmp[1]=0; tmp[2]=3; tmp[3]=0;
        }
        message = QByteArray(tmp,4);
    }
    else if (packet.data()[2]==4)
    {
        char tmp[4];
        query.exec("select cookie from tcookie where cookie = " + QString::number((int)packet.data()[3]));
        if (query.next())
        {
            query.exec("update tcookie set flag = 1 where cookie = " + QString::number((int)packet.data()[3]));
            tmp[0]=0; tmp[1]=0; tmp[2]=5; tmp[3]=0;
        }
        else
        {
            tmp[0]=0; tmp[1]=0; tmp[2]=6; tmp[3]=0;
        }
        message = QByteArray(tmp,4);
    }
    else if (packet.data()[2]==7)
    {
        // xiang ying 8

        //TODO:delMe set flag false
    }
    else if (packet.data()[2]==9)
    {
        return ;
    }
    else if (packet.data()[2]==10)
    {
        return ;
    }
    connect(this,&EZServer::sendMessage,thread,&SocketThread::sendMessage);
    emit sendMessage(message);
    disconnect(this,&EZServer::sendMessage,thread,&SocketThread::sendMessage);
}

