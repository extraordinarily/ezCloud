#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QTextBrowser>
#include<QTcpSocket>
#include<QTextEdit>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void connectServer();
    void readMessage();
    void sendMessage();

private:
    QPushButton *start_bt,*send_bt;
    QTextEdit *input_edit;
    QTextBrowser *text_browser;
    QTcpSocket *m_tcpsocket;
};

#endif // MAINWINDOW_H
