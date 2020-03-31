#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QHostAddress>

#include <QLineEdit>

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    QLineEdit *ipLineEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;

    int state;
    int cookie;

    explicit ClientSocket(QObject *parent = nullptr);
    QTcpSocket socket;

    void login();
    void moveToThreadAll(QThread *thread);

signals:
    void error(int errorCode);
};

#endif // CLIENTSOCKET_H
