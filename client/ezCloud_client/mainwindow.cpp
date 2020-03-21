#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : mainWindow_with_tray(parent)
{
    this->setCentralWidget(&wgt_ui);
    loginW.show();
    wgt_ui.setDisable(true);
    this->hide();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

