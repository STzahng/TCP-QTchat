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

QString client::getCurPath()
{
    return m_strCurPath;
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

void client::handleMsg(PDU *pdu)
{

    qDebug()<<"pdu->uiMsgType: "<<pdu->uiMsgType;
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
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        m_Msghandle->FLUSH_FRIEND_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DEL_FRIEND_RESPOND:{
        m_Msghandle->DEL_FRIEND_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{
        m_Msghandle->CHAT_REQUEST(pdu);
        break;
    }
    case ENUM_MSG_TYPE_MKDIR_RESPOND:{
        m_Msghandle->MKDIR_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:{
        m_Msghandle->FLUSH_FILE_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:{
        m_Msghandle->DEL_DIR_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DEL_FILE_RESPOND:{
        m_Msghandle->DEL_FILE_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_RENAME_RESPOND:{
        m_Msghandle->RENAME_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:{
        m_Msghandle->MOVE_FILE_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:{
        m_Msghandle->UPLOAD_FILE_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_DATA_RESPOND:{
        m_Msghandle->UPLOAD_DATA_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:{
        m_Msghandle->DOWNLOAD_FILE_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_DATA_RESPOND:{
        m_Msghandle->DOWNLOAD_DATA_RESPOND(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:{
        m_Msghandle->SHARE_FILE_RESPOND();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
        m_Msghandle->SHARE_FILE_REQUEST(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND:{
        m_Msghandle->SHARE_FILE_AGREE_RESPOND(pdu);
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
        m_strCurPath = strList.at(2);
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
    qDebug()<<"\n\nrecvMsg 接收到的消息长度： " <<m_TcpSocket.bytesAvailable();
    QByteArray data = m_TcpSocket.readAll();
    buffer.append(data);

    while(buffer.size() >= int(sizeof(PDU))){//（使用循环来处理粘包问题）至少保证buffer中的数据能读取出一个基本的PDU去获取PDU的长度
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size() < pdu->uiMsgLen){//（处理半包问题）判断是否是一个完整的PDU
            break;
        }
        handleMsg(pdu);
        buffer.remove(0,pdu->uiPDULen);
        pdu = NULL;
    }
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
