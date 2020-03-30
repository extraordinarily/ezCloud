#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    server = new EZServer();
    server->listen(QHostAddress::Any,30142);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}
