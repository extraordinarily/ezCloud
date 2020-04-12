#ifndef EZSERVER_H
#define EZSERVER_H

#include <QTcpServer>
#include <bits/stdc++.h>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "socketthread.h"


class EZServer : public QTcpServer
{
    Q_OBJECT
public:
    EZServer(QWidget * win);
    ~EZServer();

    std::list<SocketThread *> socketList;
    QSqlDatabase db;
    void dbHandler(SocketThread * thread,QByteArray packet);
    QWidget *mainwin;
    void delMe(SocketThread * thread);

protected:
    virtual void incomingConnection(qintptr handle) override;

signals:
    void sendMessage(QByteArray message);
};

#endif // EZSERVER_H
