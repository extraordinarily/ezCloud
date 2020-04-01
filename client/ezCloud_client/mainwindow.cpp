#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Debug
    loginUI.le_input[0].setText("192.168.1.3");
    loginUI.le_input[1].setText("test");
    loginUI.le_input[2].setText("en en");


    // -------------



    clientSocket.moveToThreadAll(&socketThread);
    clientSocket.ipLineEdit = &loginUI.le_input[0];
    clientSocket.usernameLineEdit = &loginUI.le_input[1];
    clientSocket.passwordLineEdit = &loginUI.le_input[2];

    connect(&loginUI.pb_login,&QPushButton::clicked,&clientSocket,&ClientSocket::login,Qt::QueuedConnection);

    connect(&clientSocket,&ClientSocket::error,this,&MainWindow::errorHandler,Qt::QueuedConnection);
    connect(&clientSocket,&ClientSocket::changeToDownload,this,&MainWindow::changeToDownload,Qt::QueuedConnection);

    socketThread.start();
    loginUI.show();
}

MainWindow::~MainWindow()
{
    socketThread.quit();
    socketThread.wait();
    // when close the app
    // do something here
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
