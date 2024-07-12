#include "reshandle.h"
#include "index.h"
#include "client.h"
//#include "chat.h"
#include "friend.h"
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
    Index::getinstance().getFriend()->showFriend(Index::getinstance().getFriend()->getFriendList());
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

void ResHandle::CHAT_REQUEST(PDU *pdu)
{
    char TarName[32] = {'\0'};
    memcpy(&TarName,pdu->caData,32);
    Chat* m_pChat = Index::getinstance().getFriend()->CreatChat(TarName);
    QString ChatMsg = QString("%1: %2\n").arg(TarName).arg(pdu->caMsg);
    m_pChat->updateshow_LE(QString(TarName),ChatMsg);
}

void ResHandle::MKDIR_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(Index::getinstance().getFile() ,"创建文件夹","文件夹创建成功");
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"创建文件夹","文件夹创建失败");

    }
    Index::getinstance().getFile()->flushFile();
}

void ResHandle::FLUSH_FILE_RESPOND(PDU *pdu)
{
    QList<FileInfo*>pFileInfoList;
    int Icount = pdu->uiMsgLen/sizeof (FileInfo);
    for(int i = 0;i<Icount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg + i*sizeof(FileInfo),sizeof(FileInfo));
        qDebug()<<pFileInfo->caName;
        pFileInfoList.append(pFileInfo);
    }
    Index::getinstance().getFile()->updatFileList(pFileInfoList);
}

void ResHandle::DEL_DIR_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(Index::getinstance().getFile(),"删除文件","删除文件夹成功");
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"删除文件","删除文件夹失败");
    }
    Index::getinstance().getFile()->flushFile();
}

void ResHandle::DEL_FILE_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(Index::getinstance().getFile(),"删除文件","删除文件成功");
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"删除文件","删除文件失败");
    }
    Index::getinstance().getFile()->flushFile();
}

void ResHandle::RENAME_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
           Index::getinstance().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"重命名文件","重命名文件失败");
    }

}

void ResHandle::MOVE_FILE_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        Index::getinstance().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"移动文件","移动文件失败");
    }
}

void ResHandle::UPLOAD_FILE_RESPOND(PDU *pdu)
{
    int ret ;
    memcpy(&ret,pdu->caData,sizeof(int));
    if(ret == 0){
        Index::getinstance().getFile()->UploadFile();
        //Index::getinstance().getFile()->flushFile();
    }else if(ret == 1){
        QMessageBox::information(Index::getinstance().getFile(),"上传文件","上传文件失败：已有文件正在上传");
    }else if(ret == -1){
        QMessageBox::information(Index::getinstance().getFile(),"上传文件","上传文件失败：无法建立对应文件");
    }
}

void ResHandle::UPLOAD_DATA_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(Index::getinstance().getFile(),"上传文件","上传文件成功");
        Index::getinstance().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"上传文件","上传文件失败：上传数据异常");
    }
}

void ResHandle::DOWNLOAD_FILE_RESPOND(PDU *pdu)
{
    int ret ;
    qint64 Filesize;
    memcpy(&Filesize,pdu->caData,sizeof(qint64));
    memcpy(&ret,pdu->caData + 32,sizeof(int));
    qDebug()<<ret<<" "<<Filesize;
    if(ret == 0){
        Index::getinstance().getFile()->DownLoadFile(Filesize);
    }else if(ret == 1){
        QMessageBox::information(Index::getinstance().getFile(),"下载文件","下载文件失败：已有文件正在下载");
    }else if(ret == -1){
        QMessageBox::information(Index::getinstance().getFile(),"下载文件","下载文件失败：服务器无法打开目标文件");
    }
}

void ResHandle::DOWNLOAD_DATA_RESPOND(PDU *pdu)
{
    Index::getinstance().getFile()->DownLoadData(pdu);
}

void ResHandle::SHARE_FILE_RESPOND()
{
    QMessageBox::information(Index::getinstance().getFile(),"分享文件","分享文件成功");
}

void ResHandle::SHARE_FILE_REQUEST(PDU *pdu)
{
    //取出发送者和发送文件名，询问是否接收
    char sendUser[32] = {'\0'};
    memcpy(sendUser,pdu->caData,32);
    QString sendFilePath = pdu->caMsg;
    int index = sendFilePath.lastIndexOf('/');
    QString sendFileName = sendFilePath.right(sendFilePath.size() - index - 1);
    int ret = QMessageBox::question(Index::getinstance().getFile(),
                            "分享文件",QString("是否接收%1发送的%2文件？").arg(sendUser).arg(sendFileName));
    if(ret != QMessageBox::Yes){
        return;
    }
    //取出当前路径作为保存路径，将发送路径和保存路径都发送给服务器
    QString savePath = Index::getinstance().getFile()->strUserPath;
    PDU* respdu = mkPDU(savePath.toStdString().size() + sendFilePath.toStdString().size() + 2);
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST;
    int sendFile_size = sendFilePath.toStdString().size() + 1;
    memcpy(respdu->caData,sendFileName.toStdString().c_str(),32);
    memcpy(respdu->caData + 32,&sendFile_size,sizeof(int));
    memcpy(respdu->caMsg,sendFilePath.toStdString().c_str(),sendFile_size);
    memcpy(respdu->caMsg + sendFile_size,savePath.toStdString().c_str(),savePath.toStdString().size());
    qDebug()<<pdu->caMsg<<endl<<pdu->caMsg + sendFile_size ;
    client::getInstance().sendPDU(respdu);
}

void ResHandle::SHARE_FILE_AGREE_RESPOND(PDU *pdu)
{
    bool ret ;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret){
        QMessageBox::information(Index::getinstance().getFile(),"分享文件","分享文件成功");
        Index::getinstance().getFile()->flushFile();
    }else{
        QMessageBox::information(Index::getinstance().getFile(),"分享文件","分享文件失败：分享数据异常");
    }
}
