#ifndef UIPACK_AS_WIDGET_LOGIN_H
#define UIPACK_AS_WIDGET_LOGIN_H

#include <QObject>
#include <QWidget>
#include "socket_defines.h"


class UIpack_as_widget_login : public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout vbl_main;
    QFormLayout fl_up;
    QHBoxLayout hbl_down;
    QLabel lb_tips[3];
    QLineEdit le_input[3];
    QPushButton pb_login;
    QPushButton pb_register;


public:
    explicit UIpack_as_widget_login(QWidget *parent = nullptr);


    void uiInit();

signals:

};

#endif // UIPACK_AS_WIDGET_LOGIN_H
