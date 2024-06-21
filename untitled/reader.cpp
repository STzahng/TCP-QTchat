#include "reader.h"
#include <QDebug>
reader::reader(QObject *parent) : QObject(parent)
{

}
void reader::slot_newspaper(){
    qDebug()<<"今日没有报纸";
}
void reader::slot_newspaper(QString str){
    qDebug()<<"今日报纸内容是："<<str;

}
