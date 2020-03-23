#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <uipack_as_widget_login.h>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>
class LoginWindow : public QMainWindow
{
    Q_OBJECT
private:
    UIpack_as_widget_login wgt_ui;
    void login();
    void heatBeat();
    QTcpSocket *socket;
    QTimer *timer;

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    bool state;
    QHostAddress serverIP;


signals:
    void changeToMain(QTcpSocket *socket);

};

#endif // LOGINWINDOW_H
