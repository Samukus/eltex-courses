#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::about(this,"hello","Hello World of QT!!!\nYour push button 1");
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about(this,"hello","Hello World of QT!!!\nYour push button 2");
}
