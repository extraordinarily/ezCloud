#ifndef CONTROLSOCKET_H
#define CONTROLSOCKET_H

#include <QThread>
#include <QTimer>
#include <QTcpSocket>
#include <bits/stdc++.h>
#include <QHostAddress>

class ControlSocket : public QThread
{
    Q_OBJECT
public:
    ControlSocket();
    ~ControlSocket();

    int cookie;
    QTcpSocket socket;
    QTimer timer;
    int flag;

    char mBuf[65536]; int len;
    QString item[600];

    void login(QString ip,int port);
    void refresh();
    void messageHandler();
    void heartBeat();
    void errorHandler();
    void stopAll();
    void sendMSG(QByteArray msg);

signals:
    void loginOver(bool succ);
    void refreshOver(int num);
    void error();

};

#endif // CONTROLSOCKET_H
