#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    clientSocket.ipLineEdit = &loginUI.le_input[0];

    connect(&loginUI.pb_login,&QPushButton::clicked,&clientSocket,&ClientSocket::login);
    connect(&clientSocket,&ClientSocket::error,this,&MainWindow::errorHandler);

    clientSocket.moveToThreadAll(&socketThread);
    socketThread.start();

    setCentralWidget(&loginUI);
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
            QMessageBox::warning(this,"warning","connecting time out");
        default:
            break;
    }
}

void MainWindow::changeToLogin()
{
    
}

void MainWindow::changeToDownload()
{

}

void MainWindow::changeToUpload()
{

}
