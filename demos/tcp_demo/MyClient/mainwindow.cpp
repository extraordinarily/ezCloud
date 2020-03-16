#include "mainwindow.h"
#include<QAction>
#include<QDateTime>
#include<QHBoxLayout>
#include<QVBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *wid=new QWidget;
    this->setCentralWidget(wid);
    QHBoxLayout *buttom_layout=new QHBoxLayout;
    QVBoxLayout *main_layout=new QVBoxLayout(wid);
    start_bt=new QPushButton("Connect",wid);
    send_bt=new QPushButton("Send",wid);
    input_edit=new QTextEdit(wid);
    text_browser=new QTextBrowser(wid);
    buttom_layout->addStretch();
    buttom_layout->addWidget(start_bt);
    buttom_layout->addStretch();
    buttom_layout->addWidget(send_bt);
    buttom_layout->addStretch();
    buttom_layout->setSpacing(100);
    text_browser->setFixedHeight(300);
    main_layout->addWidget(text_browser);
    main_layout->addSpacing(30);
    input_edit->setFixedHeight(70);
    main_layout->addWidget(input_edit);
    main_layout->addSpacing(30);
    main_layout->addLayout(buttom_layout);
    main_layout->addStretch();
    main_layout->setContentsMargins(30,30,30,30);
    this->resize(500,500);
    m_tcpsocket = new QTcpSocket;
    this->setWindowTitle("My QQ client");
    send_bt->setShortcut(Qt::Key_Enter);

    connect(start_bt,SIGNAL(clicked(bool)),this,SLOT(connectServer()));
    connect(send_bt,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));

}

MainWindow::~MainWindow()
{
}
void MainWindow::connectServer()
{
    m_tcpsocket=new QTcpSocket(this);
    m_tcpsocket->abort();
    m_tcpsocket->connectToHost("172.28.162.135",30142);
    connect(m_tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}
void MainWindow::readMessage()
{
    QByteArray qba =m_tcpsocket->readAll();
    qDebug()<<qba;
    QString ss=QVariant(qba).toString();
    text_browser->append(QDateTime::currentDateTime().time().toString()+"\n"+ss);

}
void MainWindow::sendMessage()
{
    QString ss= input_edit->toPlainText();
    m_tcpsocket->write(ss.toStdString().c_str(),strlen(ss.toStdString().c_str()));
    text_browser->append(QDateTime::currentDateTime().time().toString()+"\n"+ss);
    input_edit->clear();
}
