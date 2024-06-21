#include "client.h"
#include "ui_client.h"
#include <QDebug>
#include <QFile>
client::client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
    QFile file(":/client.config");  //固定冒号开头
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        qDebug() << strData;
        QStringList strList = strData.split("\r\n");//按\r\n拆分
        QString strIP = strList.at(0);
        quint16 usPort = strList.at(1).toUShort();
        qDebug() << "ip:" << strIP << " port:" << usPort;
        file.close();
    }
    else
    {
        qDebug() <<"打开失败";
    }

}

client::~client()
{
    delete ui;
}

