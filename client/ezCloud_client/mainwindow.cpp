#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    clientSocket.ipLineEdit = &loginUI.le_input[0];
    clientSocket.usernameLineEdit = &loginUI.le_input[1];
    clientSocket.passwordLineEdit = &loginUI.le_input[2];

    connect(&loginUI.pb_login,&QPushButton::clicked,&clientSocket,&ClientSocket::login,Qt::QueuedConnection);
    connect(&clientSocket,&ClientSocket::error,this,&MainWindow::errorHandler,Qt::QueuedConnection);
    connect(&clientSocket,&ClientSocket::changeToDownload,this,&MainWindow::changeToDownload,Qt::QueuedConnection);

    clientSocket.moveToThreadAll(&socketThread);
    socketThread.start();
    loginUI.show();
}

MainWindow::~MainWindow()
{
    socketThread.quit();
    socketThread.wait();
}

void MainWindow::errorHandler(int errorCode)
{
    switch(errorCode)
    {
        case 1:
            QMessageBox::warning(this,"warning","check you ip address!");
            break;
        case 2:
            QMessageBox::warning(this,"warning","check you user name");
            break;
        case 3:
            QMessageBox::warning(this,"warning","connecting time out");
            break;
        default:
            break;
    }
}

void MainWindow::changeToLogin()
{
    
}

void MainWindow::changeToDownload()
{
    loginUI.hide();
    downloadUI.show();

}

void MainWindow::changeToUpload()
{

}
