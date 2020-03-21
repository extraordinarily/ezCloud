#include "login_window.h"

#include <QDebug>

Login_window::Login_window(QWidget *parent) : QMainWindow(parent)
{
    this->setCentralWidget(&wgt_ui);
}

Login_window::~Login_window()
{
    qDebug() << "Login_window::Destructor Called";
}
