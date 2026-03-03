#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleButton1()
{
    QMessageBox msgBox;
    msgBox.setText("Button 1 was clicked");
    msgBox.exec();
}

void MainWindow::handleButton2()
{
    QMessageBox msgBox;
    msgBox.setText("Button 2 was clicked");
    msgBox.exec();
}
