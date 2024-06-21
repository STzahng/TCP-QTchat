#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <mybutton.h>
#include "news_paper.h"
#include "reader.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//将输入的命令传入QApplication中执行
    MainWindow w;
    news_paper p;
    reader r;
    void (news_paper::*n)() = &news_paper::signal_newparer;
    void (news_paper::*n1)(QString str) = &news_paper::signal_newparer;

    void (reader::*d)() = &reader::slot_newspaper;
    void (reader::*d1)(QString str) = &reader::slot_newspaper;

    QObject::connect(&p,n,&r,d);
    QObject::connect(&p,n1,&r,d1);
    p.send();
    p.send("hhh");
}
