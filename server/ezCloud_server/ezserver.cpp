#include "ezserver.h"

EZServer::EZServer()
{

}

EZServer::~EZServer()
{
    for (std::vector<SocketThread *>::iterator
         it = socketList.begin();it!=socketList.end();it++)
    {
        (*it)->quit();
        (*it)->wait();
        delete (*it);
    }
}

void EZServer::incomingConnection(qintptr handle)
{
    SocketThread * thread = new SocketThread(handle);
    thread->start();
    socketList.push_back(thread);
}
