#include "server.h"
#include "ui_server.h"
#include "mytcpserver.h"


#include <QFile>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    config_ip();
    MyTcpServer::getInstance().listen(QHostAddress(s_strIp),s_strPort);
}

Server::~Server()
{
    delete ui;
}

Server &Server::S_getinstance()
{
    static Server S_instance;
    return S_instance;
}

void Server::config_ip()
{
    QFile file(":/server.config.txt");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray baData = file.readAll();
        QString strData = QString(baData);

        QStringList strList = strData.split("\r\n");
        s_strIp = strList.at(0);
        s_strPort = strList.at(1).toUShort();
        qDebug()<<"server_ip: "<<s_strIp<<endl<<"server_port: "<<s_strPort;
        file.close();
    }
    else {
        qDebug()<<" 打开服务器配置文件失败";
    }
}

