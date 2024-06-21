#include "client.h"
#include "ui_client.h"
#include "protocol.h"
#include "index.h"


#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QString>

client::client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
    config_ip();
    connect(&m_TcpSocket,&QTcpSocket::connected,[](){
        qDebug()<<"连接服务器成功...";
    });
    connect(&m_TcpSocket,&QTcpSocket::readyRead,this,&client::recvMsg);
    m_TcpSocket.connectToHost((QHostAddress)m_strIP,m_strPort);
    m_Msghandle = new ResHandle;
}

client &client::getInstance()
{
    static client instance;
    return instance;
}

PDU *client::readPDU()
{
    //qDebug()<<"recvMsg 接收到的消息长度： " <<m_TcpSocket.bytesAvailable();
    uint uiPUDLen = 0;
    m_TcpSocket.read((char*)&uiPUDLen,sizeof(uint));//先读取消息的协议长度
    uint uiMsgLen = uiPUDLen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsgLen);
    m_TcpSocket.read((char*)pdu + sizeof(uint),uiPUDLen - sizeof(uint));
    qDebug()<<"uiPUDLen: "<<uiPUDLen<<endl<<"uiMsgLen: "<<uiMsgLen<<endl<<"pdu->uiMsgType: "<<pdu->uiMsgType<<endl;
    //由于之前先读取了消息的协议长度数据，所以要将指针进行偏移，读取剩余内容，同时剩余读取的长度也要减去之前的数据长度
    return pdu;
}

void client::sendPDU(PDU *pdu)
{
    qDebug()<<"pdu->uiMsgType: "<<pdu->uiMsgType
            <<"pdu->caData: "<<pdu->caData
            <<"pdu->caData+32"<<pdu->caData+32;

    //将消息传输，并释放pdu
    m_TcpSocket.write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void client::handleMeg(PDU *pdu)
{
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_RESPOND:{
        m_Msghandle->REGIST_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:{
        m_Msghandle->LOGIN_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FIND_USER_RESPOND:{
        m_Msghandle->FIND_USER_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ONLINE_USER_RESPOND:{
        m_Msghandle->ONLINE_USER_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
        m_Msghandle->ADD_FRIEND_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        m_Msghandle->ADD_FRINED_REQUEST(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND:{
        m_Msghandle->ADD_FRIEND_AGREE_RESPOND();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:{
        m_Msghandle->ADD_FRIEND_AGREE_REQUEST();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        m_Msghandle->FLUSH_FRIEND_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DEL_FRIEND_RESPOND:{
        m_Msghandle->DEL_FRIEND_RESPOND(pdu);
        break;
    }
    default: break;
    }
}

QString client::getLoginName()
{
    return m_StrLoginName;
}

client::~client()
{
    delete ui;
}

void client::config_ip()
{
    QFile file(":/client.config.txt");  //固定冒号开头
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = QString(baData);

        QStringList strList = strData.split("\r\n");//按\r\n拆分
        m_strIP = strList.at(0);
        m_strPort = strList.at(1).toUShort();
        qDebug() << "client_ip: " << m_strIP << endl << "client_port: " << m_strPort;
        file.close();
    }
    else
    {
        qDebug() <<"打开客户端配置文件失败";
    }
}


void client::on_regist_PB_clicked()
{
    //获取输入框中的用户名和密码
    qDebug()<<"客户端点击注册按钮...";
    QString strName = ui->username_LE->text();
    QString strPwd = ui->pwd_LE->text();
    //如果用户名和密码不合规，弹窗警告
    if(strName.isEmpty()||strPwd.isEmpty()||strName.size()>32||strPwd.size()>32){
        QMessageBox::critical(this,"注册","用户名或密码错误");
        return ;
    }
    //创建pdu，由于数据都存入caDate中不需要额外申请空间，所以为零
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);

    sendPDU(pdu);

}

void client::recvMsg()
{
    PDU* pdu = readPDU();
    handleMeg(pdu);
    free(pdu);
    pdu = NULL;
}

void client::on_login_PB_clicked()
{
    //获取输入框中的用户名和密码
    qDebug()<<"客户端点击登录按钮...";
    QString strName = ui->username_LE->text();
    QString strPwd = ui->pwd_LE->text();
    //如果用户名和密码不合规，弹窗警告
    if(strName.isEmpty()||strPwd.isEmpty()||strName.size()>32||strPwd.size()>32){
        QMessageBox::critical(this,"登录","用户名或密码非法");
        return ;
    }
    //创建pdu，由于数据都存入caDate中不需要额外申请空间，所以为零
    m_StrLoginName = strName;
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
    sendPDU(pdu);

}

QTcpSocket& client::getTcpSocket()
{
    return m_TcpSocket;
}
