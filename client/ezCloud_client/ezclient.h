#ifndef EZCLIENT_H
#define EZCLIENT_H

#include "ui_header.h"
#include "controlsocket.h"

#include <QObject>
#include <QDir>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>

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

signals:
    void csLogin(QString ip,int port);
    void csRefresh();
};
#endif // EZCLIENT_H
