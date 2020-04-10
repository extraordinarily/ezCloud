#include "ezclient.h"
#include <QDebug>
EZClient::EZClient(QString path) : path(path)
{
    showLoginUI();
}

EZClient::~EZClient()
{
    controlSocket.quit();
    controlSocket.wait();
}

void EZClient::showLoginUI()
{
    QDir dir(path);
    QFile file(dir.absoluteFilePath("cookie"));
    if (file.exists())
    {
        loginUI.lb[2].setText("   已注册   ");
        loginUI.pb_register.setDisabled(true);
        file.open(QFile::ReadOnly|QFile::Text);
        QTextStream in(&file);
        in >> controlSocket.cookie;
        file.close();
    }
    else
    {
        loginUI.lb[2].setText("   未注册   ");
        loginUI.pb_login.setDisabled(true);
    }
    loginUI.ipAddr.setText("192.168.1.3");
    loginUI.show();

    connect(&loginUI.pb_login,&QPushButton::clicked,this,&EZClient::login);
    connect(&loginUI.pb_register,&QPushButton::clicked,this,&EZClient::regis);

    connect(this,&EZClient::csLogin,&controlSocket,&ControlSocket::login,Qt::QueuedConnection);
    connect(&controlSocket,&ControlSocket::loginOver,this,&EZClient::loginOver);
}

void EZClient::login()
{
    emit csLogin(loginUI.ipAddr.text(),30142);
    loginUI.pb_login.setDisabled(true);
}

void EZClient::loginOver(bool succ)
{
    if (succ)
    {
        connect(&controlSocket,&ControlSocket::error,this,&EZClient::errorHandler);
        loginUI.hide();
        connect(&uploadUI.pb_refresh,&QPushButton::clicked,this,&EZClient::refresh);
        connect(this,&EZClient::csRefresh,&controlSocket,&ControlSocket::refresh,Qt::QueuedConnection);
        connect(&controlSocket,&ControlSocket::refreshOver,this,&EZClient::refreshOver);
        connect(&uploadUI.tbv_upload,&QTableView::clicked,this,&EZClient::tableViewClicked);

        connect(&uploadUI.pb_download,&QPushButton::clicked,this,&EZClient::download);
        connect(&uploadUI.pb_delete,&QPushButton::clicked,this,&EZClient::deleteItem);
        connect(&uploadUI.pb_upload,&QPushButton::clicked,this,&EZClient::upload);

        refresh();
        uploadUI.show();
    }
    else
    {
        loginUI.pb_login.setDisabled(false);
        QMessageBox::warning(&loginUI,"失败","登录失败");
    }
}

void EZClient::download()
{
    // qDebug()<<uploadUI.currentIndex;
    downloadUI.show();
    // xia zai socket qi dong.
}
void EZClient::deleteItem()
{
    // qDebug()<<uploadUI.currentIndex;
    // emit fa bao
}
void EZClient::upload()
{
    QString fileName=QFileDialog::getOpenFileName();
    // 计算MD5
    // emit 上传
}

void EZClient::refreshOver(int num)
{
    uploadUI.model.clear();
    uploadUI.model.setHorizontalHeaderLabels({"filename","MD5","cookie","ip","length","zaixian"});
    for (int i=0;i<num;i++)
    for (int j=0;j<6;j++)
    uploadUI.model.setItem(i,j,new QStandardItem(controlSocket.item[i*6+j]));
}

void EZClient::refresh()
{
    emit csRefresh();
}

void EZClient::regis()
{
    QTcpSocket socket;
    socket.connectToHost(loginUI.ipAddr.text(),30142);
    if (!socket.waitForConnected()) {socket.close(); return ;}
    char buf[4]; buf[0]=0; buf[1]=0; buf[2]=1; buf[3]=0;
    socket.write(buf,4);
    if (!socket.waitForBytesWritten()) {socket.close(); return ;}
    socket.flush();
    if (!socket.waitForReadyRead()) {socket.close(); return ;}
    memcpy(buf,socket.readAll(),4);
    if (buf[2]==2)
    {
        QDir dir(path);
        QFile file(dir.absoluteFilePath("cookie"));
        file.open(QFile::WriteOnly|QFile::Text);
        QTextStream out(&file);
        out<<(int) buf[3];
        file.flush();
        file.close();
        controlSocket.cookie = (int) buf[3];
        loginUI.pb_register.setDisabled(true);
        loginUI.pb_login.setDisabled(false);
        loginUI.lb[2].setText("   已注册   ");
        QMessageBox::warning(&loginUI,"成功","注册被接受");
    }
    else
    {
        QMessageBox::warning(&loginUI,"失败","注册被拒接");
    }
    socket.close();
}

void EZClient::errorHandler()
{
    QMessageBox::warning(&loginUI,"掉线","您已掉线");
    uploadUI.close();
}

void EZClient::tableViewClicked(const QModelIndex& index)
{
    uploadUI.currentIndex = index.row();
}
