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
    std::chrono::steady_clock::time_point active;
    QTimer timer;
    QTcpSocket socket;

    explicit ClientSocket(QObject *parent = nullptr);

    void moveToThreadAll(QThread *thread);

    void login();
    void logout();
    void messageHandler();
    void heatBeatHandler();

signals:
    void error(int errorCode);
    void changeToDownload();
};

#endif // CLIENTSOCKET_H
