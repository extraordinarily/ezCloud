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
    connect(server,&QTcpServer::newConnection,this,&MainWindow::connectingHandler);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectingHandler()
{
    socket = server->nextPendingConnection();
    conn
    // create another thread here
/*
    if 登录请求 返回行不行
    else if 注册请求 返回行不行
    else if 心跳检测 ACK
    else if 列表拉取
*/
}
