#ifndef EZCLIENT_H
#define EZCLIENT_H

#include "ui_header.h"
#include "controlsocket.h"

#include <QObject>
#include <QDir>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QTcpServer>
#include <QCryptographicHash>

class EZClient : public QObject
{
    Q_OBJECT

public:
    EZClient(QString path);
    ~EZClient();

    QString path;
    UIpack_as_widget_login loginUI;
    UIpack_as_widget_upload uploadUI;
    UIpack_as_widget_download downloadUI;
    ControlSocket controlSocket;

    QByteArray rbuffer;
    QTcpServer fileServer;
    QTcpSocket *sender;
    QTcpSocket *receiver;

    void showLoginUI();

    void regis();
    void login();
    void refresh();

    void loginOver(bool succ);
    void refreshOver(int num);
    void errorHandler();

    void tableViewClicked(const QModelIndex& index);

    void download();
    void deleteItem();
    void upload();

    long long flen;
    QFile * localFile;
    long long rlen;
    QFile * rFile;
    void handleConnection();
    void handleSender();
    void handleNCon();
    void sendFile(qint64 num);

    void freceive();
    void recNCon();


signals:
    void csLogin(QString ip,int port);
    void csRefresh();
    void sendMSG(QByteArray msg);
};
#endif // EZCLIENT_H
