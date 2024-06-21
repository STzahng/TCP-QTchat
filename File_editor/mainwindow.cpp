#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QIcon>
#include <QDialog>
#include <QLabel>
#include <QFontComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QTextEdit>
#include <QTextFormat>
#include <QColor>
#include <QColorDialog>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1200,800);
    this->setWindowTitle("无标题文档");
    this->setWindowIcon(QIcon(":/RES/app.png"));
    set_MenuBar();
    set_ToolBar();
    set_central();
    set_StatusBar();
}
void MainWindow::set_MenuBar(){
    MenuBar = ui->menubar;
    //设置文件菜单
    set_FileMenu();
    //设置编辑菜单
    set_EditMenu();
    //设置帮助菜单
    set_helpMenu();
}
void MainWindow::set_FileMenu(){
    FileMenu = MenuBar->addMenu("文件");

    //添加活动项
    open_Act = FileMenu->addAction(QIcon(":/RES/openfile.png"),"打开文件");
    new_Act = FileMenu->addAction(QIcon(":/RES/createfile.png"),"新建文件");
    save_Act = FileMenu->addAction(QIcon(":/RES/savefile.png"),"保存文件");
    Save_File();
    Open_File();
    New_File();
    QAction *Exit_Act = FileMenu->addAction(QIcon(":/RES/exit.png"),"退出");
    QObject::connect(Exit_Act,&QAction::triggered,save_Act,&QAction::triggered);
    QObject::connect(Exit_Act,&QAction::triggered,[this](){
       this->close();
    });
}
void MainWindow::set_EditMenu(){
    EditMenu = MenuBar->addMenu("编辑");
    Copy_Act = this->EditMenu->addAction(QIcon(":/RES/copy.png"),"复制");
    QObject::connect(Copy_Act,&QAction::triggered,[this](){
       TextEdit->copy();
    });
    Paste_Act = this->EditMenu->addAction(QIcon(":/RES/paste.png"),"粘贴");
    QObject::connect(Paste_Act,&QAction::triggered,[this](){
       TextEdit->paste();
    });
    Cut_Act = this->EditMenu->addAction(QIcon(":/RES/cut.png"),"剪切");
    QObject::connect(Cut_Act,&QAction::triggered,[this](){
       TextEdit->cut();
    });
    Cancel_Act = this->EditMenu->addAction(QIcon(":/RES/cancel.png"),"撤销");
    QObject::connect(Cancel_Act,&QAction::triggered,[this](){
       TextEdit->setUndoRedoEnabled(true);
       TextEdit->undo();
    });
    Resume_Act = this->EditMenu->addAction(QIcon(":/RES/resume.png"),"反撤销");
    QObject::connect(Resume_Act,&QAction::triggered,[this](){
       TextEdit->setUndoRedoEnabled(true);
       TextEdit->redo();
    });
}
void MainWindow::set_helpMenu(){
    helpMenu = MenuBar->addMenu("帮助");
}
void MainWindow::set_ToolBar(){
   ToolBar =new QToolBar(this);
   ToolBar->setFixedHeight(40);
   this->addToolBar(ToolBar);

   set_Font();//设置文字
   ToolBar->addSeparator();
   set_FontSize();//设置文字大小
   ToolBar->addSeparator();
   set_FontBlod();//设置文字加粗
   ToolBar->addSeparator();
   set_FontItalic();//设置文字斜体
   ToolBar->addSeparator();
   set_FontColor();//设置文字颜色
   ToolBar->addSeparator();
   //添加按钮链接活动项
   ToolBar->addAction(Copy_Act);
   ToolBar->addAction(Paste_Act);
   ToolBar->addAction(Cut_Act);
   ToolBar->addAction(Cancel_Act);
   ToolBar->addAction(Resume_Act);

}
void MainWindow::set_StatusBar(){
    QStatusBar *StatusBar = new QStatusBar(this);
    this->setStatusBar(StatusBar);

    QLabel *L = new QLabel("时间",this);
    QLabel *time = new QLabel(this);
    QTimer *timer = new QTimer(this);

    StatusBar->addWidget(L);
    StatusBar->addWidget(time);
    QObject::connect(timer,&QTimer::timeout,[=](){
        QString t = QDateTime::currentDateTime().toString();
        time->setText(t);
    });
    timer->start(1000);
}
void MainWindow::set_central(){
    TextEdit = new QTextEdit();
    this->setCentralWidget(TextEdit);
    QFont font("宋体",15);//初始化控件的默认大小1
    TextEdit->setFont(font);
    QObject::connect(TextEdit,&QTextEdit::textChanged,[this](){
        if(!this->windowTitle().startsWith("*"))
        {
            this->setWindowTitle("*"+ this->windowTitle());
        }
    });
}
void MainWindow::set_Font(){
    QFontComboBox *Fontcombo = new QFontComboBox(ToolBar);
    QLabel *Label = new QLabel("字体： ");
    ToolBar->addWidget(Label);
    ToolBar->addWidget(Fontcombo);
    QObject::connect(Fontcombo,&QFontComboBox::currentFontChanged,[this](const QFont &f){
       this->TextEdit->setFont(f);
    });
}
void MainWindow::set_FontSize(){
    QComboBox* ComboBoxSize = new QComboBox(ToolBar);
    for(int i = 15;i<=30;i++){
        ComboBoxSize->addItem(QString::number(i));
    }
    QLabel* Label = new QLabel("字号： ");
    ToolBar->addWidget(Label);
    ToolBar->addWidget(ComboBoxSize);
    QObject::connect(ComboBoxSize,&QComboBox::currentTextChanged,[this](const QString &size){
       this->TextEdit->setFontPointSize(size.toDouble());
    });
}
void MainWindow::set_FontBlod(){
    QPushButton* BlodBnt = new QPushButton(QIcon(":/RES/blod.png"),"",ToolBar);
    ToolBar->addWidget(BlodBnt);
    BlodBnt->setFixedSize(QSize(35,35));
    QObject::connect(BlodBnt,&QPushButton::clicked,[this](){
       TextEdit->setFontWeight(TextEdit->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    });
}
void MainWindow::set_FontItalic(){
    QPushButton* ItalicBnt = new QPushButton(QIcon(":/RES/italic.png"),"",ToolBar);
    ToolBar->addWidget(ItalicBnt);
    ItalicBnt->setFixedSize(QSize(35,35));
    QObject::connect(ItalicBnt,&QPushButton::clicked,[this](){
       TextEdit->setFontItalic(!TextEdit->fontItalic());
    });
}
void MainWindow::set_FontColor(){
    QPushButton* ColorBnt = new QPushButton(QIcon(":/RES/color.png"),"",ToolBar);
    ToolBar->addWidget(ColorBnt);
    ColorBnt->setFixedSize(QSize(35,35));
    QObject::connect(ColorBnt,&QPushButton::clicked,[this](){
       QColor FontColor = QColorDialog::getColor();
       //接收文字格式的对象接收颜色
       QTextCharFormat FontFormat;
       FontFormat.setForeground(QBrush(FontColor));
       TextEdit->mergeCurrentCharFormat(FontFormat);
    });
}



void MainWindow::Open_File(){
    QObject::connect(open_Act,&QAction::triggered,save_Act,&QAction::triggered);
    QObject::connect(open_Act,&QAction::triggered,[this](){
       QString OpenFileName = QFileDialog::getOpenFileName();
       QFile file(OpenFileName);
       if(!file.open(QIODevice::ReadWrite))return;
       QTextStream stream(&file);
       QString str = stream.readAll();
       TextEdit->setText(str);
       this->setWindowTitle(OpenFileName);
    });

}
void MainWindow::New_File(){
    QObject::connect(new_Act,&QAction::triggered,save_Act,&QAction::triggered);
    QObject::connect(new_Act,&QAction::triggered,[this](){
       TextEdit->clear();
       this->setWindowTitle("无标题文档");
    });
}
void MainWindow::Save_File(){
    QObject::connect(save_Act,&QAction::triggered,[this](){
        if(!this->windowTitle().startsWith("*"))return;//若未修改过内容则保存直接返回
        if(QMessageBox::question(this,"保存","是否保存？") == QMessageBox::No)return;
        QString FileName;
        if(this->windowTitle() == "*无标题文档"){//判断文件是否是第一次保存
            FileName= QFileDialog::getSaveFileName(this);
        }
        else{
            QStringList strL = this->windowTitle().split("*");
            FileName = strL[1];
        }
        QFile* file = new QFile(FileName);//创建文件对象
        if(!file->open(QIODevice::ReadWrite))return;//以读写的方式打开文件 如果打开失败结束
        //获取中心控件上的文字
        QString str = this->TextEdit->toPlainText();

        QTextStream TextStream(file);
        TextStream<<str;//将str字符串写入到file文件中
        file->close();
        this->setWindowTitle((FileName));//设置文件名字为 路径
    });
}



MainWindow::~MainWindow()
{
    delete ui;
}

