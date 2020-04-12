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

        connect(this,&EZClient::sendMSG,&controlSocket,&ControlSocket::sendMSG);

        fileServer.listen(QHostAddress::Any,30143);
        connect(&fileServer,&QTcpServer::newConnection,this,&EZClient::handleConnection);

        uploadUI.currentIndex = -1;
        refresh();
        uploadUI.show();
    }
    else
    {
        loginUI.pb_login.setDisabled(false);
        QMessageBox::warning(&loginUI,"失败","登录失败");
    }
}

void EZClient::handleNCon()
{
    qDebug()<<"发完了";
    localFile->close();
    delete localFile;
    qDebug()<<"文件关掉";
    disconnect(sender,&QTcpSocket::readyRead,this,&EZClient::handleSender);
    disconnect(sender,&QTcpSocket::bytesWritten,this,&EZClient::sendFile);
    sender->close();
    delete sender;
    qDebug()<<"sender关掉";
    downloadUI.hide();
    qDebug()<<"取消显示";
}

void EZClient::handleSender()
{
    QByteArray tmp = sender->readAll();
    rbuffer.append(tmp);
    if (rbuffer.length()<192) return ;

    std::string tfname(rbuffer.data(),rbuffer.data()+strlen(rbuffer.data()));
    rbuffer.clear();
    localFile = new QFile(QString::fromStdString(tfname));
    if (!localFile->open(QFile::ReadOnly)) {
        sender->close(); return ;
    }
    flen = localFile->size();
    sendFile(0);
}

void EZClient::sendFile(qint64 num)
{
    flen-=num;
    if (flen==0) {sender->flush();handleNCon(); return ;}
    QByteArray tmp = localFile->read(65536);
    sender->write(tmp);
}

void EZClient::handleConnection()
{
    sender = fileServer.nextPendingConnection();
    connect(sender,&QTcpSocket::readyRead,this,&EZClient::handleSender);
    connect(sender,&QTcpSocket::bytesWritten,this,&EZClient::sendFile);
    downloadUI.show();
}

void EZClient::download()
{
    int ind = uploadUI.currentIndex;
    downloadUI.show();
    receiver = new QTcpSocket();
    receiver->connectToHost(controlSocket.item[ind*6+3],30143);
    receiver->waitForConnected();

    char buf[192];
    QString fname = controlSocket.item[ind*6];
    strcpy(buf,fname.toStdString().c_str());
    int len = strlen(buf);
    for (int i=len;len<192;len++) buf[i] = 0;
    QByteArray msg(buf,192);

    rlen = controlSocket.item[ind*6+4].toLongLong();
    QDir dir(path);
    rFile = new QFile(dir.absoluteFilePath(fname.split("/").last()));
    rFile->open(QFile::WriteOnly);

    connect(receiver,&QTcpSocket::readyRead,this,&EZClient::freceive);

    receiver->write(msg);
    receiver->waitForBytesWritten();
    receiver->flush();
}

void EZClient::freceive()
{
    QByteArray tmp = receiver->readAll();
    rlen-=tmp.length();
    rFile->write(tmp);
    if (rlen==0) {
        recNCon();
    }
}

void EZClient::recNCon()
{
    disconnect(receiver,&QTcpSocket::readyRead,this,&EZClient::freceive);
    receiver->close();
    delete receiver;
    rFile->close();
    delete rFile;
    downloadUI.hide();
}

void EZClient::deleteItem()
{
    char buf[196];
    if (uploadUI.currentIndex==-1) return ;
    QString fname = controlSocket.item[uploadUI.currentIndex*6];
    QString ccc = controlSocket.item[uploadUI.currentIndex*6+2];
    strcpy(buf+4,fname.toStdString().c_str());
    int len = strlen(buf+4);
    for (int i=len+4;len<196;len++) buf[i] = 0;
    buf[3] = ccc.toInt();
    buf[2] = 9;
    buf[1] = 0xc0;
    buf[0] = 0;
    QByteArray msg(buf,196);
    emit sendMSG(msg);
}

QString fileMd5(const QString &sourceFilePath) {

    QFile sourceFile(sourceFilePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

void EZClient::upload()
{
    char tmp[260];
    char *buf=tmp+4;
    QString fileName=QFileDialog::getOpenFileName();
    if (fileName=="") return ;
    QString md5 = fileMd5(fileName);
    QFile sourceFile(fileName);
    qint64 fileSize = sourceFile.size();

    // 192 filename
    // 56  MD5
    // 4   MB
    // 4   BB
    strcpy(buf,fileName.toStdString().c_str());
    int len = strlen(buf);
    for (int i=len;i<192;i++) buf[i]=0;
    strcpy(buf+192,md5.toStdString().c_str());
    for (int i=192+32;i<192+56;i++) buf[i]=0;

    int BB = fileSize%(1<<20); *(int *)(buf+252) = BB;
    int MB = fileSize/(1<<20); *(int *)(buf+248) = MB;
    qDebug()<<MB;
    qDebug()<<BB;
    qDebug()<<fileSize;
    qDebug()<<"";
    tmp[0] = 1;
    tmp[1] = 0;
    tmp[2] = 10;
    tmp[3] = controlSocket.cookie;
    QByteArray msg(tmp,260);
    emit sendMSG(msg);
}

void EZClient::refreshOver(int num)
{
    uploadUI.model.clear();
    uploadUI.model.setHorizontalHeaderLabels({"文件名","MD5","cookie","IP","文件大侠","在线状态"});
    for (int i=0;i<num;i++)
    {
        uploadUI.model.setItem(i,0,new QStandardItem(controlSocket.item[i*6].split("/").last()));
        for (int j=1;j<6;j++)
        uploadUI.model.setItem(i,j,new QStandardItem(controlSocket.item[i*6+j]));
    }
}

void EZClient::refresh()
{
    uploadUI.currentIndex = -1;
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
