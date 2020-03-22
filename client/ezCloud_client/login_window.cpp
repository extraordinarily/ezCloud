#include "login_window.h"

#include <QDebug>

Login_window::Login_window(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(&wgt_ui);
    socket = new QTcpSocket(this);
    connect(&wgt_ui.pb_login,&QPushButton::clicked,this,&Login_window::login);
}

Login_window::~Login_window()
{
}

void Login_window::login()
{
    qDebug()<<"fuck";
    //login here
    socket->abort();
    socket->connectToHost("192.168.1.5",30142);
    socket->write("hahaha",6);

    changeToMain(socket);
}
