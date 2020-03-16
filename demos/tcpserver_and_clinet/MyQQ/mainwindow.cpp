#include "mainwindow.h"
#include<QDateTime>
#include<QAction>
#include<QHostAddress>
#include<QHBoxLayout>
#include<QVBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *wid=new QWidget;
    this->setCentralWidget(wid);
    QHBoxLayout *buttom_layout=new QHBoxLayout;
    QVBoxLayout *main_layout=new QVBoxLayout(wid);
    start_bt=new QPushButton("Start");
    send_bt=new QPushButton("Send");
    input_edit=new QTextEdit();
    text_browser=new QTextBrowser();
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
    this->setWindowTitle("My QQ Server");
    send_bt->setShortcut(Qt::Key_Enter);

    //=============以上为界面设置===========
    m_tcpserver=new QTcpServer(this);
    m_tcpsocket = new QTcpSocket;
    connect(start_bt,SIGNAL(clicked(bool)),this,SLOT(startTcpserver()));
    connect(send_bt,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));
    this->show();


}

MainWindow::~MainWindow()
{

}
void  MainWindow::startTcpserver()
{
    m_tcpserver->listen(QHostAddress::Any,30142);//监听所有ip的端口30142
    qDebug()<<"start server";
    //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样，其实你可以随便取。
    connect(m_tcpserver,SIGNAL(newConnection()),this,SLOT(newConnect()));
}
void MainWindow::newConnect()
{
    m_tcpsocket = m_tcpserver->nextPendingConnection(); //得到每个连进来的socket
    qDebug()<<"new Connect";
    text_browser->append(QString("%0").arg(m_tcpsocket->peerAddress().toIPv4Address())+" is connected");
    connect(m_tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽
}
void MainWindow::readMessage()
{
    QByteArray qba= m_tcpsocket->readAll(); //读取
    qDebug()<<qba;

    QString ss=QVariant(qba).toString();
    text_browser->append(QDateTime::currentDateTime().time().toString()+"\n"+ss);
}
void MainWindow::sendMessage()
{
    QString strMesg= input_edit->toPlainText();
    qDebug()<<strMesg;
    input_edit->clear();
    m_tcpsocket->write(strMesg.toUtf8()); //发送
    text_browser->append(QDateTime::currentDateTime().time().toString()+"\n"+strMesg);
}


