#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    
    server = new EZServer();
    server->listen(QHostAddress::Any,30142);

    connect(&pb_clientTableInit,&QPushButton::clicked,this,&MainWindow::fileTableInit);
    connect(&pb_fileTableInit,&QPushButton::clicked,this,&MainWindow::clientTableInit);


    this->dbInit();
    this->uiInit();
}

void MainWindow::uiInit()
{
    this->setCentralWidget(&wgt_main);
    wgt_main.setLayout(&hbl_main);
    hbl_main.addLayout(&vbl_left);
    vbl_left.addWidget(&pb_clientTableInit);
    vbl_left.addWidget(&pb_fileTableInit);
    hbl_main.addWidget(&tbv_file);
    hbl_main.addWidget(&tbv_client);


    pb_clientTableInit.setText("Client Table Init");
    pb_fileTableInit.setText("File Table Init");

    return;
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete server;
}

void MainWindow::dbInit()
{
    this->db.close();
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(qstr_dbname);
    if (!this->db.open()) {
        qDebug() << "Error: MainWindow::dbInit Failed to connect database." << this->db.lastError();
    }else{
        qDebug() << "MainWindow::dbInit Succeed to connect database." ;
    }

    this->refreshTableView(&db, mdl_file, &tbv_file, qstr_tbname_file);
    this->refreshTableView(&db, mdl_client, &tbv_client, qstr_tbname_client);

    return;
}

void MainWindow::fileTableInit()
{
    QSqlQuery sql_query_temp;
    QString cmd;

    cmd = "drop table " + qstr_tbname_file;
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::fileTableInit Fail to delete table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::fileTableInit Table deleted!";
    }

    cmd = "create table " + qstr_tbname_file + "(fileLength int, fileName text, MD5 text, location text, onlineFlag int)";
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::fileTableInit Fail to create table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::fileTableInit Table created!";
    }



    this->refreshTableView(&db, mdl_file, &tbv_file, qstr_tbname_file);
}


void MainWindow::clientTableInit()
{
    QSqlQuery sql_query_temp;
    QString cmd;

    cmd = "drop table " + qstr_tbname_client;
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::ClientTableInit Fail to delete table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::ClientTableInit Table deleted!";
    }

    cmd = "create table " + qstr_tbname_client + "(username text, password text, cookie text)";
    qDebug() << cmd;
    if(!sql_query_temp.exec(cmd)){
        qDebug() << "Error: MainWindow::ClientTableInit Fail to create table."<< sql_query_temp.lastError();
    }else{
        qDebug() << "MainWindow::ClientTableInit Table created!";
    }

    this->refreshTableView(&db, mdl_client, &tbv_client, qstr_tbname_client);
}

void MainWindow::refreshTableView(QSqlDatabase* db, QSqlTableModel* md, QTableView* tbv, QString tbName)
{
    if(md != nullptr)
    {
        delete md;
    }
    md = new QSqlTableModel(this, *db);
    md->setTable(tbName);
    md->setEditStrategy(QSqlTableModel::OnRowChange);

    tbv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbv->setModel(md);
    tbv->setSelectionBehavior(QAbstractItemView::SelectRows);
    md->select();

    return;
}

void MainWindow::fileTableInsert(std::string p_str[], const int rowNum)
{
    QSqlQuery sql_query_temp;
    QString cmd;

    int rowLength = 3;

    QString qstr_tmp0;
    QString qstr_tmp1;
    QString qstr_tmp2;
    QString qstr_tmp3;
    QString qstr_tmp4;
    for(int i = 0; i < rowNum; i++)
    {
        qstr_tmp0 = QString::fromStdString(p_str[i*rowLength + 0]);
        qstr_tmp1 = "'" + QString::fromStdString(p_str[i*rowLength + 1]) + "'";
        qstr_tmp2 = "'" + QString::fromStdString(p_str[i*rowLength + 2]) + "'";
        qstr_tmp3 = "'" + QString::fromStdString(p_str[i*rowLength + 3]) + "'";
        qstr_tmp4 = QString::fromStdString(p_str[i*rowLength + 4]);

        cmd = "insert into " + qstr_tbname_file + " values(" + qstr_tmp0 + "," + qstr_tmp1 + "," + qstr_tmp2  + "," + qstr_tmp3 + "," + qstr_tmp4 + ")";

        qDebug() << cmd;
        if(!sql_query_temp.exec(cmd)){
            qDebug() << "Error: MainWindow::fileTableInsert Fail to insert."<< sql_query_temp.lastError();
        }else{
            qDebug() << "MainWindow::fileTableInsert insert success!";
        }
    }

    return;
}


void MainWindow::clientTableInsert(std::string p_str[], const int rowNum)
{
    QSqlQuery sql_query_temp;
    QString cmd;

    int rowLength = 3;

    QString qstr_tmp0;
    QString qstr_tmp1;
    QString qstr_tmp2;
    for(int i = 0; i < rowNum; i++)
    {
        qstr_tmp0 = "'" + QString::fromStdString(p_str[i*rowLength + 0]) + "'";
        qstr_tmp1 = "'" + QString::fromStdString(p_str[i*rowLength + 1]) + "'";
        qstr_tmp2 = "'" + QString::fromStdString(p_str[i*rowLength + 2]) + "'";

        cmd = "insert into " + qstr_tbname_client + " values(" + qstr_tmp0 + "," + qstr_tmp1 + "," + qstr_tmp2 + ")";

        qDebug() << cmd;
        if(!sql_query_temp.exec(cmd)){
            qDebug() << "Error: MainWindow::clientTableInsert Fail to insert."<< sql_query_temp.lastError();
        }else{
            qDebug() << "MainWindow::clientTableInsert insert success!";
        }
    }

    return;
}





