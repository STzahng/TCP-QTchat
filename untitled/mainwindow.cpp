#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMenuBar>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenuBar* menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("文件");
    QMenu *editMenu = menuBar->addMenu("编辑");
    QMenu *helpMenu = menuBar->addMenu("帮助");

}

MainWindow::~MainWindow()
{
    delete ui;


}

