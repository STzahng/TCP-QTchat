#include "msghandle.h"
#include "operatedb.h"
#include "mytcpsocket.h"
#include "mytcpserver.h"
#include <QDebug>
MsgHandle::MsgHandle()
{

}

PDU* MsgHandle::REGIST(PDU *pdu)
{
    //处理注册请求
    qDebug()<<"case:REGIST_REQUEST";
    //读取cadata中的用户名和密码
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    //数据库处理注册
    bool ret = OperateDb::getInstance().handleRegist(caName,caPwd);
    qDebug()<<"ret: "<<ret;
    //构建respdu，将注册结果返回给客户端
    PDU* respdu = mkPDU(0);

    respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));

    qDebug()<<"respdu->uiMsgType: "<<respdu->uiMsgType<<endl
            <<"ret: "<<ret
            <<"respdu->caData"<<respdu->caData;
    return respdu;
}

PDU* MsgHandle::LOGIN(PDU *pdu, QString &m_strLoginname)
{
    //处理登录请求
    qDebug()<<"case:REGIST_REQUEST";
    //读取cadata中的用户名和密码
    char caName[32] = {'\0'};
    char caPwd[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    //数据库处理注册
    bool ret = OperateDb::getInstance().handleLogin(caName,caPwd);
    m_strLoginname = caName;
    qDebug()<<"ret: "<<ret;
    //构建respdu，将注册结果返回给客户端
    PDU* respdu = mkPDU(0);

    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));

    qDebug()<<"respdu->uiMsgType: "<<respdu->uiMsgType<<endl
            <<"ret: "<<ret
            <<"respdu->caData"<<respdu->caData;
    return respdu;
}

PDU* MsgHandle::FindUser(PDU *pdu)
{
    //处理查找在线用户请求
    qDebug()<<"case:FindUser_REQUEST";
    //读取cadata中的用户名
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    //数据库处理注册
    int ret = OperateDb::getInstance().handleFindUser(caName);
    qDebug()<<"finduser_ret: "<<ret;
    //构建respdu，将注册结果返回给客户端
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_FIND_USER_RESPOND;
    memcpy(respdu->caData,&caName,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}

PDU* MsgHandle::OnlineUser(PDU *pdu)
{
    //处理在线用户列表请求
    qDebug()<<"case:OnlineUser_REQUEST";
    //数据库处理注册
    QStringList ret = OperateDb::getInstance().handleOnlineUser();

    //构建respdu，将注册结果返回给客户端
    //每一个用户名占用32位大小，所以申请的空间大小为个数乘32
    PDU* respdu = mkPDU(ret.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_RESPOND;
    for(int i = 0;i<ret.size();i++){
        qDebug()<<ret.at(i);

        memcpy(respdu->caMsg + i * 32,ret.at(i).toStdString().c_str(),32);
    }
    return respdu;
}

PDU *MsgHandle::AddFriend(PDU *pdu)
{
    //处理添加好友请求
    qDebug()<<"case:AddFriend_REQUEST";
    //数据库处理注册
    char CurName[32] = {'\0'};
    char TarName[32] = {'\0'};
    memcpy(CurName,pdu->caData,32);
    memcpy(TarName,pdu->caData+32,32);
    int ret = OperateDb::getInstance().handleAddFriend(CurName,TarName);
    if(ret == 1){
        MyTcpServer::getInstance().resend(TarName,pdu);
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandle::AddFriendAgree(PDU *pdu)
{
    //处理同意好友请求
    qDebug()<<"case:AddFriendAgree_REQUEST";
    //数据库处理注册
    char CurName[32] = {'\0'};
    char TarName[32] = {'\0'};
    memcpy(CurName,pdu->caData,32);
    memcpy(TarName,pdu->caData+32,32);
    OperateDb::getInstance().handleAddFriendAgree(CurName,TarName);
    MyTcpServer::getInstance().resend(CurName,pdu);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND;
    return respdu;
}

PDU *MsgHandle::FlushFriend(PDU *pdu)
{
    //处理刷新好友
    qDebug()<<"case:OnlineFriend_REQUEST";
    //数据库处理注册
    char caName[32] = {'\0'};
    memcpy(caName,pdu->caData,32);
    QStringList ret = OperateDb::getInstance().handleFlushFriend(caName);
    //构建respdu，将结果返回给客户端
    //每一个用户名占用32位大小，所以申请的空间大小为个数乘32
    PDU* respdu = mkPDU(ret.size()*32);
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
    for(int i = 0;i<ret.size();i++){
        qDebug()<<ret.at(i);
        memcpy(respdu->caMsg + i * 32,ret.at(i).toStdString().c_str(),32);
    }
    return respdu;
}

PDU *MsgHandle::DelFriend(PDU *pdu)
{
    //处理删除好友请求
    qDebug()<<"case:DelFriend_REQUEST";
    //数据库处理注册
    char CurName[32] = {'\0'};
    char TarName[32] = {'\0'};
    memcpy(CurName,pdu->caData,32);
    memcpy(TarName,pdu->caData+32,32);
    int ret = OperateDb::getInstance().handleDelFriend(CurName,TarName);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_RESPOND;
    memcpy(respdu->caData,TarName,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}
