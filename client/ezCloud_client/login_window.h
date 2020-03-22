#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <uipack_as_widget_login.h>

class Login_window : public QMainWindow
{
    Q_OBJECT
private:
    UIpack_as_widget_login wgt_ui;
    void login();
    QTcpSocket *socket;

public:
    explicit Login_window(QWidget *parent = nullptr);
    ~Login_window();


signals:
    void changeToMain(QTcpSocket *socket);

};

#endif // LOGIN_WINDOW_H
