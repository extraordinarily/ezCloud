#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setCentralWidget(&wgt_ui);

}

MainWindow::~MainWindow()
{
    //delete ui;
}

