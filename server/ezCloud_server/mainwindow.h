#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <string>

#include <QTableView>
#include <QtSql/QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>


#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>


#include "ezserver.h"

/*
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    std::string test_1[6] = {"2", "name1", "1", "3", "name1", "2"};

    QString qstr_dbname = "server";
    QString qstr_tbname_file = "file";
    QString qstr_tbname_client = "client";
    QSqlDatabase db;
    QSqlTableModel* mdl_file = nullptr;
    QSqlTableModel* mdl_client = nullptr;

    int currentRowNum_file;
    int currentRowNum_client;
    QSqlRecord rcd_current_file;
    QSqlRecord rcd_current_client;

    void dbInit();
    void fileTableInit();
    void clientTableInit();
    void dbClear();

    void refreshTableView(QSqlDatabase*, QSqlTableModel*, QTableView*, QString);

    void fileTableInsert(std::string[], const int);
    void clientTableInsert(std::string[], const int);



    QWidget wgt_main;
    QHBoxLayout hbl_main;
    QVBoxLayout vbl_left;
    QPushButton pb_clientTableInit;
    QPushButton pb_fileTableInit;

    QTableView tbv_file;
    QTableView tbv_client;


private:
    void uiInit();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Ui::MainWindow *ui;
    EZServer *server;

private slots:
};
#endif // MAINWINDOW_H
