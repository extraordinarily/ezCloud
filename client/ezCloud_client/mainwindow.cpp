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

    connect(&downloadUI.tbv_download, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableViewClicked_download(const QModelIndex &)));
    connect(&uploadUI.tbv_upload, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableViewClicked_upload(const QModelIndex &)));

    clientSocket.moveToThreadAll(&socketThread);

    downloadWindow.setCentralWidget(&downloadUI);
    downloadWindow.show();

    socketThread.start();
    //loginUI.show();
    //uploadUI.show();
    win.setCentralWidget(&uploadUI);
    win.show();
    this->dbInit();
    this->uploadTableRefresh(test, 2);
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






void MainWindow::dbInit()
{
    this->db.close();
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(qstr_dbname);
    if (!this->db.open()) {
        qDebug() << "Error: tableViewInit(QTableView* tbv) Failed to connect database." << this->db.lastError();
    }else{
        qDebug() << "tableViewInit(QTableView* tbv) Succeed to connect database." ;
    }

    return;
}

void MainWindow::downloadTableRefresh(std::string p_str[], const int rowNum)
{
    QSqlQuery sql_query_temp;
    QString cmd;

    cmd = "drop table " + qstr_tbname_download;
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::downloadTableRefresh Fail to delete table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::downloadTableRefresh Table deleted!";
    }

    cmd = "create table " + qstr_tbname_download + "(fileLength int, fileName text)";
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::downloadTableRefresh Fail to create table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::downloadTableRefresh Table created!";
    }

    int rowLength = 3;
    QString qstr_tmp0;
    QString qstr_tmp1;
    for(int i = 0; i < rowNum; i++)
    {
        qstr_tmp0 = QString::fromStdString(p_str[i*rowLength + 0]);
        qstr_tmp1 = "'" + QString::fromStdString(p_str[i*rowLength + 1]) + "'";
        cmd = "insert into " + qstr_tbname_download + " values(" + qstr_tmp0 + "," + qstr_tmp1 + ")";
        qDebug() << cmd;
        if(!sql_query_temp.exec(cmd)){
            qDebug() << "Error: MainWindow::downloadTableRefresh Fail to insert."<< sql_query_temp.lastError();
        }else{
            qDebug() << "MainWindow::downloadTableRefresh insert success!";
        }
    }



    if(mdl_download != nullptr)
    {
        delete mdl_download;
    }
    mdl_download = new QSqlTableModel(this, db);
    mdl_download->setTable(qstr_tbname_download);
    mdl_download->setEditStrategy(QSqlTableModel::OnRowChange);

    mdl_download->select();
    downloadUI.tbv_download.setEditTriggers(QAbstractItemView::NoEditTriggers);
    downloadUI.tbv_download.setModel(mdl_download);
    downloadUI.tbv_download.setSelectionBehavior(QAbstractItemView::SelectRows);

    return;
}

void MainWindow::uploadTableRefresh(std::string p_str[], const int rowNum)
{
    QSqlQuery sql_query_temp;
    QString cmd;

    cmd = "drop table " + qstr_tbname_upload;
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::uploadTableRefresh Fail to delete table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::uploadTableRefresh Table deleted!";
    }

    cmd = "create table " + qstr_tbname_upload + "(fileLength int, fileName text)";
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::uploadTableRefresh Fail to create table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::uploadTableRefresh Table created!";
    }

    int rowLength = 3;
    QString qstr_tmp0;
    QString qstr_tmp1;
    for(int i = 0; i < rowNum; i++)
    {
        qstr_tmp0 = QString::fromStdString(p_str[i*rowLength + 0]);
        qstr_tmp1 = "'" + QString::fromStdString(p_str[i*rowLength + 1]) + "'";

        cmd = "insert into " + qstr_tbname_upload + " values(" + qstr_tmp0 + "," + qstr_tmp1 + ")";

        qDebug() << cmd;
        if(!sql_query_temp.exec(cmd)){
            qDebug() << "Error: MainWindow::uploadTableRefresh Fail to insert."<< sql_query_temp.lastError();
        }else{
            qDebug() << "MainWindow::uploadTableRefresh insert success!";
        }
    }



    if(mdl_upload != nullptr)
    {
        delete mdl_upload;
    }
    mdl_upload = new QSqlTableModel(this, db);
    mdl_upload->setTable(qstr_tbname_upload);
    mdl_upload->setEditStrategy(QSqlTableModel::OnRowChange);

    mdl_upload->select();
    uploadUI.tbv_upload.setEditTriggers(QAbstractItemView::NoEditTriggers);
    uploadUI.tbv_upload.setModel(mdl_upload);
    uploadUI.tbv_upload.setSelectionBehavior(QAbstractItemView::SelectRows);

    return;
}


void MainWindow::tableViewClicked_download(const QModelIndex& index)
{
    currentRowNum_download = index.row();
    rcd_current_download = mdl_download->record(currentRowNum_download);
    qDebug() << currentRowNum_download;
    qDebug() << rcd_current_upload.value(0).toString() << rcd_current_upload.value(1).toString();
}

void MainWindow::tableViewClicked_upload(const QModelIndex& index)
{
    currentRowNum_upload = index.row();
    rcd_current_upload = mdl_upload->record(currentRowNum_upload);
    qDebug() << currentRowNum_upload;
    qDebug() << rcd_current_upload.value(0).toString() << rcd_current_upload.value(1).toString();
    downloadWindow.show();
}
