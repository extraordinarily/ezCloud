#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <uipack_as_widget_login.h>
#include <uipack_as_widget_upload.h>
#include <uipack_as_widget_download.h>
#include <mainwindow_with_tray.h>
#include <login_window.h>


class MainWindow : public mainWindow_with_tray
{
    Q_OBJECT

private:
    //UIpack_as_widget_login wgt_ui;
    //UIpack_as_widget_download wgt_ui;
    UIpack_as_widget_upload wgt_ui;

private:
    Login_window loginW;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
