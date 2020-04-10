#ifndef FILESOCKET_H
#define FILESOCKET_H

#include <QThread>

class FileSocket : public QThread
{
    Q_OBJECT
public:
    FileSocket();
};

#endif // FILESOCKET_H
