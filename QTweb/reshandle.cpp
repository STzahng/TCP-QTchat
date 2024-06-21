#include "reshandle.h"
#include "index.h"
#include "client.h"
#include <QMessageBox>

ResHandle::ResHandle()
{

}

void ResHandle::REGIST_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&client::getInstance(),"注册","注册成功");
    }else{
        QMessageBox::information(&client::getInstance(),"注册","注册失败");

    }
}

void ResHandle::LOGIN_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(&client::getInstance(),"登录","登录成功");
        Index::getinstance().show();
        client::getInstance().hide();
    }else{
        QMessageBox::information(&client::getInstance(),"登录","登录失败");

    }
}

void ResHandle::FIND_USER_RESPOND(PDU *pdu)
{
    int ret ;
    char caName[32] = {'\0'};
    memcpy(&caName,pdu->caData,32);
    memcpy(&ret,pdu->caData+32,sizeof(int));
    if(ret == -1){
        QMessageBox::information(&client::getInstance(),"搜索",QString("%1 不存在").arg(caName));

    }else if(ret == 0){
        QMessageBox::information(&client::getInstance(),"搜索",QString("%1 不在线").arg(caName));
    }
    else{
        int ret = QMessageBox::information(&client::getInstance(),"搜索",QString("%1 在线").arg(caName),
                                 "添加好友","取消");
        if(ret == 0){
            PDU* pdu =mkPDU(0);
            memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
            memcpy(pdu->caData + 32, caName,32);
            pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
            client::getInstance().sendPDU(pdu);
        }

    }
}

void ResHandle::ONLINE_USER_RESPOND(PDU *pdu)
{
    int uiSize = pdu->uiMsgLen/32;
    char caTemp[32];
    QStringList userList;
    for(int i = 0;i < uiSize;i++){
        memcpy(caTemp,pdu->caMsg + i * 32,32);
        if(QString(caTemp) != client::getInstance().getLoginName())
        userList.append(QString(caTemp));
    }
    Index::getinstance().getFriend()->getOnlineUser()->showOnlineUser(userList);

}

void ResHandle::ADD_FRIEND_RESPOND(PDU *pdu)
{
    int ret;
    memcpy(&ret, pdu->caData,sizeof(int));
    if(ret == -1){
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","添加失败");
    }else if(ret == -2){
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","已经是好友，无需添加");

    }else if(ret == 0){
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","对方不在线，请等对方上线后再添加");

    }else {
        QMessageBox::information(Index::getinstance().getFriend(),"添加好友","好友申请以发出，等待对方同意");

    }
}

void ResHandle::ADD_FRINED_REQUEST(PDU *pdu)
{
    char CaName[32] = {'\0'};
    memcpy(&CaName,pdu->caData,32);
    int ret = QMessageBox::question(Index::getinstance().getFriend(),"添加好友",
                                    QString("是否同意 %1 添加好友申请？").arg(CaName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;
    memcpy(respdu->caData,pdu->caData,64);
    client::getInstance().sendPDU(respdu);

}

void ResHandle::ADD_FRIEND_AGREE_RESPOND()
{
    QMessageBox::information(Index::getinstance().getFriend(),"添加好友","添加好友成功");
}

void ResHandle::ADD_FRIEND_AGREE_REQUEST()
{
    QMessageBox::information(Index::getinstance().getFriend(),"添加好友","添加好友成功");
}

void ResHandle::FLUSH_FRIEND_RESPOND(PDU *pdu)
{
    int uiSize = pdu->uiMsgLen/32;
    char caTemp[32];
    Index::getinstance().getFriend()->getFriendList().clear();
    for(int i = 0;i < uiSize;i++){
        memcpy(caTemp,pdu->caMsg + i * 32,32);
        //qDebug()<<QString(caTemp);
        Index::getinstance().getFriend()->getFriendList().append(QString(caTemp));
    }
    Index::getinstance().getFriend()->showFriend(Index::getinstance().getFriend()->getFriendList());
}

void ResHandle::DEL_FRIEND_RESPOND(PDU *pdu)
{
    int ret;
    char TarName[32] = {'\0'};
    memcpy(TarName,pdu->caData,32);
    memcpy(&ret, pdu->caData + 32,sizeof(int));
    if(ret == 0){
        QMessageBox::information(Index::getinstance().getFriend(),"删除好友","删除失败");
    }else if(ret == -2){
        QMessageBox::information(Index::getinstance().getFriend(),"删除好友","你和对方不是好友");

    }else if(ret == 1){
        QMessageBox::information(Index::getinstance().getFriend(),"删除好友","删除好友成功");
        QStringList friendList= Index::getinstance().getFriend()->getFriendList();

        for(auto it = friendList.begin();it != friendList.end();++it){
            qDebug()<<*it;
            if(*it == TarName){
                friendList.erase(it);
                break;
            }
        }
        Index::getinstance().getFriend()->getFriendList() = friendList;
        Index::getinstance().getFriend()->showFriend(friendList);

    }
}
