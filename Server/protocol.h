#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef unsigned int uint;
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    //注册
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPOND,
    //登录
    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPOND,
    //查找用户
    ENUM_MSG_TYPE_FIND_USER_REQUEST,
    ENUM_MSG_TYPE_FIND_USER_RESPOND,
    //在线用户
    ENUM_MSG_TYPE_ONLINE_USER_REQUEST,
    ENUM_MSG_TYPE_ONLINE_USER_RESPOND,
    //添加用户
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,
    //同意添加用户
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND,
    //刷新好友
    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,
    //删除好友
    ENUM_MSG_TYPE_DEL_FRIEND_REQUEST,
    ENUM_MSG_TYPE_DEL_FRIEND_RESPOND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

struct PDU{//协议数据单元结构体

    uint uiPDULen;//协议总长度
    uint uiMsgLen;//实际消息长度
    uint uiMsgType;//消息类型
    char caData[64];//参数
    char caMsg[];//实际消息
};

PDU* mkPDU(uint uiMsgLen);
#endif // PROTOCOL_H
