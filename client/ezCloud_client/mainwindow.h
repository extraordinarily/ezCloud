#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDialog>
#include <QThread>
#include <string>
#include <QTableView>
#include <QtSql/QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "ui_header.h"
#include "clientsocket.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    std::string test[6] = {"2", "name1", "1", "3", "name1", "2"};

    QString qstr_dbname = "tmpDB";
    QString qstr_tbname_upload = "up";
    QString qstr_tbname_download = "down";
    QSqlDatabase db;
    QSqlTableModel* mdl_download = nullptr;
    QSqlTableModel* mdl_upload = nullptr;

    int currentRowNum_download;
    int currentRowNum_upload;
    QSqlRecord rcd_current_download;
    QSqlRecord rcd_current_upload;

    void dbInit();
    void downloadTableRefresh(std::string[], const int);
    void uploadTableRefresh(std::string[], const int);

public:
    UIpack_as_widget_login loginUI;
    UIpack_as_widget_download downloadUI;
    UIpack_as_widget_upload uploadUI;

    QMainWindow downloadWindow;



    QThread socketThread;
    ClientSocket clientSocket;

    void changeToLogin();
    void changeToDownload();
    void changeToUpload();
    void errorHandler(int errorCode);

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void tableViewClicked_download(const QModelIndex& index);
    void tableViewClicked_upload(const QModelIndex& index);

};
#endif // MAINWINDOW_H
