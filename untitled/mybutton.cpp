#include "mybutton.h"
#include <QPushButton>
#include <QDebug>
MyButton::MyButton(QWidget *parent) : QPushButton(parent)
{
    qDebug()<<"bnt构造";
}
MyButton::~MyButton(){
    qDebug()<<"bnt析构";

}
