#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QObject>
#include <uipack_as_widget_login.h>

class Login_window : public QMainWindow
{
    Q_OBJECT
private:
    UIpack_as_widget_login wgt_ui;

public:
    explicit Login_window(QWidget *parent = nullptr);
    ~Login_window();


signals:

};

#endif // LOGIN_WINDOW_H
