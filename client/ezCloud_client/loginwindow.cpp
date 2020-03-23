#include "loginwindow.h"

#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(&wgt_ui);
    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    state = false;
    connect(&wgt_ui.pb_login,&QPushButton::clicked,this,&LoginWindow::login);
    connect(timer,&QTimer::timeout,this,&LoginWindow::heatBeat);
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::heatBeat()
{
    //handle it hear
}

void LoginWindow::login()
{
    if (!state)
    {
        if (!serverIP.setAddress(wgt_ui.le_input[0].text()))
        {
            QMessageBox::warning(this,"error","please check ip address!");
            return ;
        }
        if (wgt_ui.le_input[1].text()=="")
        {
            QMessageBox::warning(this,"error","please input username!");
            return ;
        }
        socket->abort();
        socket->connectToHost(serverIP,30142);
        socket->write("hahaha",6);
    }
    changeToMain(socket);
}
