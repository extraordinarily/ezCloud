#ifndef EZSERVER_H
#define EZSERVER_H

#include <QTcpServer>
#include <bits/stdc++.h>
#include "socketthread.h"

class EZServer : public QTcpServer
{
public:
    EZServer();
    ~EZServer();

    std::vector<SocketThread *> socketList;

protected:
    virtual void incomingConnection(qintptr handle) override;
};

#endif // EZSERVER_H
