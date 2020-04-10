#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = new EZServer(this);
    server->listen(QHostAddress::Any,30142);
}

MainWindow::~MainWindow()
{
    delete server;
}
