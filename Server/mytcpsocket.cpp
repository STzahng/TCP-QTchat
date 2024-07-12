#include "mytcpsocket.h"
#include "operatedb.h"
#include "protocol.h"

#include <QString>
MyTcpSocket::MyTcpSocket()
{
    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
    connect(this,&QTcpSocket::disconnected,this,&MyTcpSocket::clientOffline);
    m_Msghandle = new MsgHandle;
}


MyTcpSocket::~MyTcpSocket()
{
    delete m_Msghandle;
}

PDU *MyTcpSocket::readpdu()
{
    qDebug()<<"recvMsg 接收到的消息长度： " <<this->bytesAvailable();
    uint uiPUDLen = 0;
    this->read((char*)&uiPUDLen,sizeof(uint));//先读取消息的协议长度
    uint uiMsgLen = uiPUDLen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsgLen);
    //由于之前先读取了消息的协议长度数据，所以要将指针进行偏移，读取剩余内容，同时剩余读取的长度也要减去之前的数据长度
    this->read((char*)pdu + sizeof(uint),uiPUDLen - sizeof(uint));
    return pdu;
}

PDU* MyTcpSocket::handleMsg(PDU *pdu)
{

    qDebug()<<"pdu->uiMsgType: "<<pdu->uiMsgType;
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_REQUEST:{
        return m_Msghandle->REGIST(pdu);
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:{
        return m_Msghandle->LOGIN(pdu,m_strLoginname);
    }
    case ENUM_MSG_TYPE_FIND_USER_REQUEST:{
        return m_Msghandle->FindUser(pdu);
    }
    case ENUM_MSG_TYPE_ONLINE_USER_REQUEST:{
        return m_Msghandle->OnlineUser(pdu);
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        return m_Msghandle->AddFriend(pdu);
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:{
        return m_Msghandle->AddFriendAgree(pdu);
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:{
        return m_Msghandle->FlushFriend(pdu);
    }
    case ENUM_MSG_TYPE_DEL_FRIEND_REQUEST:{
        return m_Msghandle->DelFriend(pdu);
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{
         m_Msghandle->Chat(pdu);
         return NULL;
    }
    case ENUM_MSG_TYPE_MKDIR_REQUEST:{
        return m_Msghandle->MKDir(pdu);
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:{
        return m_Msghandle->FlushFile(pdu);
    }
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:{
        return m_Msghandle->DelDir(pdu);
    }
    case ENUM_MSG_TYPE_DEL_FILE_REQUEST:{
        return m_Msghandle->DelFile(pdu);
    }
    case ENUM_MSG_TYPE_RENAME_REQUEST:{
        return m_Msghandle->ReName(pdu);
    }
    case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:{
        return m_Msghandle->MoveFile(pdu);
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:{
        return m_Msghandle->UploadFile(pdu);
    }
    case ENUM_MSG_TYPE_UPLOAD_DATA_REQUEST:{
        return m_Msghandle->UploadData(pdu);
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:{
        return m_Msghandle->DownloadFile(pdu);
    }
    case ENUM_MSG_TYPE_DOWNLOAD_DATA_REQUEST:{
        return m_Msghandle->DownloadData(pdu,this);
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:{
        return m_Msghandle->ShareFile(pdu);
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:{
        return m_Msghandle->ShareFileAgree(pdu);
    }
    default:
        break;
    }
    return NULL;
}

void MyTcpSocket::  sendPDU(PDU *pdu)
{
    qDebug()<<"sendpdu";
    if(pdu == NULL){
        return ;
    }
    write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<"\n\nPDU处理的消息长度： " <<this->bytesAvailable();
    QByteArray data = this->readAll();
    buffer.append(data);

    while(buffer.size() >= int(sizeof(PDU))){//（使用循环来处理粘包问题）至少保证buffer中的数据能读取出一个基本的PDU去获取PDU的长度
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size()<pdu->uiMsgLen){//（处理半包问题）判断是否是一个完整的PDU
            break;
        }
        sendPDU(handleMsg(pdu));
        buffer.remove(0,pdu->uiPDULen);
        pdu = NULL;
    }

}

void MyTcpSocket::clientOffline()
{
    OperateDb::getInstance().handleOffline(m_strLoginname.toStdString().c_str());
    emit Offline(this);
}
