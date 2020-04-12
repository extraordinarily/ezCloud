#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <bits/stdc++.h>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr handle);

    QTcpSocket *socket;
    char buf[65536];
    int len;
    int ip;

    void run() override;

    void messageHandler();
    void sendMessage(QByteArray message);
    void error();

signals:
    void delMe(SocketThread * thread);
    void dbHandler(SocketThread * thread,QByteArray packet);
};

#endif // SOCKETTHREAD_H
