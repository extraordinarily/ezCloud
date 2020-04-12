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
    query.exec("create table if not exists tcookie (cookie integer primary key autoincrement,flag integer,ip integer)");
    query.exec("create table if not exists tfile (filename text, md5 text, cookie integer, MB integer, BB integer)");
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

void EZServer::delMe(SocketThread *thread)
{
    for (std::list<SocketThread *>::iterator
         it = socketList.begin();it!=socketList.end();it++)
    if (thread == (*it))
    {
        (*it)->quit();
        (*it)->wait();
        delete (*it);
        socketList.erase(it);
        return ;
    }
}

void EZServer::incomingConnection(qintptr handle)
{
    SocketThread * thread = new SocketThread(handle);
    connect(thread,&SocketThread::delMe,this,&EZServer::delMe);
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
            query.exec("insert into tcookie (flag, ip) values (0, 0)");
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
            query.exec("update tcookie set flag = 1, ip = " + QString::number(thread->ip)
                       + " where cookie = " + QString::number((int)packet.data()[3]));
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
        QByteArray tmsg;
        query.exec("select * from tfile natural join tcookie");
        while (query.next())
        {
            // 192 filename
            // 50  MD5
            // 1   cookie
            // 1   flag
            // 4   ip
            // 4   MB
            // 4   BB
            char buf[256];
            strcpy(buf,query.value(0).toString().toStdString().c_str());
            int len = strlen(buf);
            for (int i=len;i<192;i++) buf[i]=0;
            strcpy(buf+192,query.value(1).toString().toStdString().c_str());
            for (int i=192+32;i<192+50;i++) buf[i]=0;
            buf[192+50] = query.value(2).toInt();
            buf[192+51] = query.value(5).toInt();
            *(int *)(buf+244) = query.value(6).toInt();
            *(int *)(buf+248) = query.value(4).toInt();
            *(int *)(buf+252) = query.value(5).toInt();
            QByteArray msg(buf,256);
            tmsg.append(msg);
        }
        char tmp[4];
        tmp[0] = tmsg.length()/256;
        tmp[1] = tmsg.length()%256;
        tmp[2] = 8;
        tmp[3] = 0;
        message = QByteArray(tmp,4);
        message.append(tmsg);
    }
    else if (packet.data()[2]==9)
    {
        int cookie = packet.data()[3];
        std::string filename(packet.data()+4,packet.data()+4+strlen(packet.data()+4));
        query.exec("delete from tfile where filename = \""
                   + QString::fromStdString(filename) + "\" and cookie = "
                   + QString::number(cookie));
        return ;
    }
    else if (packet.data()[2]==10)
    {
        std::string filename(packet.data()+4,packet.data()+4+strlen(packet.data()+4));
        std::string md5(packet.data()+196,packet.data()+196+strlen(packet.data()+196));
        int cookie = packet.data()[3];
        int MB = *(int *)(packet.data()+252);
        int BB = *(int *)(packet.data()+256);
        query.exec("insert into tfile (filename, md5, cookie, MB, BB) values ( \""
                   + QString::fromStdString(filename) + "\" , \""
                   + QString::fromStdString(md5) + "\" , "
                   + QString::number(cookie) + " , "
                   + QString::number(MB) + " , "
                   + QString::number(BB) + " )");
        return ;
    }
    connect(this,&EZServer::sendMessage,thread,&SocketThread::sendMessage);
    emit sendMessage(message);
    disconnect(this,&EZServer::sendMessage,thread,&SocketThread::sendMessage);
}

