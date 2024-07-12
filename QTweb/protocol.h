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
    //聊天
    ENUM_MSG_TYPE_CHAT_REQUEST,
    ENUM_MSG_TYPE_CHAT_RESPOND,
    //创建文件夹
    ENUM_MSG_TYPE_MKDIR_REQUEST,
    ENUM_MSG_TYPE_MKDIR_RESPOND,
    //刷新文件
    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND,
    //删除文件夹
    ENUM_MSG_TYPE_DEL_DIR_REQUEST,
    ENUM_MSG_TYPE_DEL_DIR_RESPOND,
    //删除文件
    ENUM_MSG_TYPE_DEL_FILE_REQUEST,
    ENUM_MSG_TYPE_DEL_FILE_RESPOND,
    //重命名
    ENUM_MSG_TYPE_RENAME_REQUEST,
    ENUM_MSG_TYPE_RENAME_RESPOND,
    //移动文件
    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,
    //上传文件
    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND,
    //上传数据
    ENUM_MSG_TYPE_UPLOAD_DATA_REQUEST,
    ENUM_MSG_TYPE_UPLOAD_DATA_RESPOND,
    //下载文件
    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,
    //下载数据
    ENUM_MSG_TYPE_DOWNLOAD_DATA_REQUEST,
    ENUM_MSG_TYPE_DOWNLOAD_DATA_RESPOND,
    //分享文件
    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,
    //同意分享文件
    ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST,
    ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

struct FileInfo{
    char caName[32];
    int Filetype;//0代表文件夹 1代表文件
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