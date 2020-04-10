#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <bits/stdc++.h>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr handle);

    QTcpSocket *socket;
    char buf[65536];
    int len;

    void run() override;

    void messageHandler();
    void sendMessage(QByteArray message);

signals:
    void delMe();
    void dbHandler(SocketThread * thread,QByteArray packet);
};

#endif // SOCKETTHREAD_H
