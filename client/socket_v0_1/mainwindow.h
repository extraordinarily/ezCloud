#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <uipack_as_widget_login.h>
#include <uipack_as_widget_upload.h>
#include <uipack_as_widget_download.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //UIpack_as_widget_login wgt_ui;
    UIpack_as_widget_download wgt_ui;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
