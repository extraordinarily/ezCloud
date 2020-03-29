#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDialog>
#include <QThread>
#include "ui_header.h"
#include "clientsocket.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    UIpack_as_widget_login loginUI;
    UIpack_as_widget_download downloadUI;
    UIpack_as_widget_upload uploadUI;

    QThread socketThread;
    ClientSocket clientSocket;

    void changeToLogin();
    void changeToDownload();
    void changeToUpload();
    void errorHandler(int errorCode);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
