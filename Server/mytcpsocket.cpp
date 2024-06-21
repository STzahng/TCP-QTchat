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
    qDebug()<<"uiPUDLen: "<<uiPUDLen<<endl
            <<"uiMsgLen: "<<uiMsgLen<<endl
            <<"pdu->uiMsgType: "<<pdu->uiMsgType<<endl
           <<"pdu->caMsg: "<<pdu->caMsg;
    return pdu;
}

PDU* MyTcpSocket::handleMsg(PDU *pdu)
{

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
    default:
        break;
    }
    return NULL;
}

void MyTcpSocket::sendPDU(PDU *pdu)
{
    if(pdu == NULL){
        return ;
    }
    write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::recvMsg()
{
    PDU* pdu = readpdu();
    sendPDU(handleMsg(pdu));
    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::clientOffline()
{
    OperateDb::getInstance().handleOffline(m_strLoginname.toStdString().c_str());
    emit Offline(this);
}
