#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
   // , ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    this->setCentralWidget(&wgt_ui);

}

MainWindow::~MainWindow()
{
    //delete ui;
}

