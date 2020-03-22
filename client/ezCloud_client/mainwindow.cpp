#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : mainWindow_with_tray(parent)
{
    connect(&loginW,&Login_window::changeToMain,this,&MainWindow::changeToMain);
    connect(&wgt_ui.pb_logout,&QPushButton::clicked,this,&MainWindow::changeToLogin);
    connect(&wgt_ui.pb_refresh,&QPushButton::clicked,this,&MainWindow::refreshList);
    this->setCentralWidget(&wgt_ui);
    changeToLogin();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::changeToLogin()
{
    wgt_ui.setDisable(true);
    this->hide();
    // heart beat stop
    loginW.show();
}

void MainWindow::changeToMain(QTcpSocket *socket)
{
    this->socket = socket;
    wgt_ui.setDisable(false);
    this->show();
    // heart beat begin
    loginW.hide();
}

void MainWindow::refreshList()
{
    // connect to server
    socket->write("fresh",5);
}
