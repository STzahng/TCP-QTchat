#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置菜单栏
    {
    QMenuBar* menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);
    QMenu *fileMenu = menuBar->addMenu("文件");
    QMenu *editMenu = menuBar->addMenu("编辑");
    QMenu *helpMenu = menuBar->addMenu("帮助");
    fileMenu->addAction("打开");
    fileMenu->addAction("保存");
    fileMenu->addAction("新建");
    QAction *exit = fileMenu->addAction("退出");
    }
    //设置状态栏
    {
    QStatusBar *statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);
    QLabel *bal = new QLabel("time:",statusBar);
    statusBar -> addWidget(bal);
    statusBar -> addWidget(new QLabel("行:",statusBar));
    statusBar -> addWidget(new QLabel("列:",statusBar));
    statusBar -> insertWidget(0,new QPushButton("状态栏:",statusBar));
    }
    //锚接部件
    {
//        QDockWidget *dock = new QDockWidget("锚接部件",this);
//        this->addDockWidget(Qt::LeftDockWidgetArea,dock);
    }
    //
}

MainWindow::~MainWindow()
{
    delete ui;
}

