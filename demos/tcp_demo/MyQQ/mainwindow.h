#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QTcpServer>
#include<QTcpSocket>
#include<QTextBrowser>
#include<QTextEdit>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startTcpserver();
    void sendMessage();
    void newConnect();
    void readMessage();
private:
    QPushButton *start_bt,*send_bt;
    QTextEdit *input_edit;
    QTextBrowser *text_browser;
    QTcpServer *m_tcpserver;
    QTcpSocket *m_tcpsocket;

};

#endif // MAINWINDOW_H
