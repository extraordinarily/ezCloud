#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,30142);
    connect(server,&QTcpServer::newConnection,this,&MainWindow::messageHandler);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::messageHandler()
{
    socket = server->nextPendingConnection();
    // create another thread here
    QString message = QVariant(socket->readAll()).toString();
    qDebug()<<message;
/*
    if 登录请求 返回行不行
    else if 注册请求 返回行不行
    else if 心跳检测 ACK
    else if 列表拉取
*/
}
