#include "msghandle.h"
#include "operatedb.h"
#include "server.h"
#include "mytcpserver.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
MsgHandle::MsgHandle()
{

    m_bUpload = false;
    m_bDownload = false;
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
    if(ret){
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::S_getinstance().getRootPath()).arg(caName));
    }
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
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND;
    MyTcpServer::getInstance().resend(CurName,respdu);
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
    qDebug()<<ret;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_RESPOND;
    memcpy(respdu->caData,TarName,32);
    memcpy(respdu->caData+32,&ret,sizeof(int));
    return respdu;
}

void MsgHandle::Chat(PDU *pdu)
{
    //处理聊天请求
    qDebug()<<"case:Chat_REQUEST";
    //数据库处理注册
    char TarName[32] = {'\0'};
    memcpy(TarName,pdu->caData+32,32);
    MyTcpServer::getInstance().resend(TarName,pdu);
}

PDU *MsgHandle::MKDir(PDU *pdu)
{
    //处理创建文件夹请求
    qDebug()<<"case:MKDir_REQUEST";
    QString strcurPath = pdu->caMsg;
    qDebug()<<strcurPath<<endl<<pdu->caMsg;
    QString strNewPath = QString("%1/%2").arg(strcurPath).arg(pdu->caData);
    bool ret = true;
    QDir dir;
    if(!dir.exists(strcurPath) || dir.exists(strNewPath) || !dir.mkdir(strNewPath)){
        qDebug()<<"!dir.exists(strcurPath))";
        ret = false;
    }
    PDU* respdu = mkPDU(0);
    qDebug()<<ret;
    respdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    qDebug()<<respdu->caData;
    return respdu;

}

PDU *MsgHandle::FlushFile(PDU *pdu)
{
    qDebug()<<"case:FlushFile_REQUEST";
    QString strPath = pdu->caMsg;
    QDir dir(strPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    int filecount = fileInfoList.size();
    PDU* respdu = mkPDU(sizeof(FileInfo) * (filecount - 2));//除去‘.’‘..’文件
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
    FileInfo* pFileInfo = NULL;
    for(int i = 2;i < filecount;i++){
        QString strFileName = fileInfoList[i].fileName();
        pFileInfo = (FileInfo*)(respdu->caMsg) + i-2;
        memcpy(pFileInfo->caName,strFileName.toStdString().c_str(),32);
        if(fileInfoList[i].isDir()){
            pFileInfo->Filetype = 0;//目录
        }else{
            pFileInfo->Filetype = 1;//文件
        }

    }
    return respdu;
}

PDU *MsgHandle::DelDir(PDU *pdu)
{
    qDebug()<<"case:DelDir_REQUEST";
    qDebug()<<pdu->caMsg;
    QFileInfo fileInfo(pdu->caMsg);
    bool ret = false;
    if(fileInfo.isDir()){
        QDir dir(pdu->caMsg);
        ret = dir.removeRecursively();
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;

}

PDU *MsgHandle::DelFile(PDU *pdu)
{
    qDebug()<<"case:DelFile_REQUEST";
    QFileInfo fileInfo(pdu->caMsg);
    bool ret = false;
    if(fileInfo.isFile()){
        qDebug()<<"\nyes";
        QFile file(pdu->caMsg);
        //QDir dir(pdu->caMsg);
        ret = file.remove();
    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *MsgHandle::ReName(PDU *pdu)
{
    qDebug()<<"case:ReName_REQUEST";
    char OldName[32] = {'\0'};
    char NewName[32] = {'\0'};
    memcpy(OldName,pdu->caData,32);
    memcpy(NewName,pdu->caData + 32,32);
    char *path = pdu->caMsg;
    QString OldPath = QString("%1/%2").arg(path).arg(OldName);
    QString NewPath = QString("%1/%2").arg(path).arg(NewName);
    QDir dir;
    bool ret = dir.rename(OldPath,NewPath);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_RENAME_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;

}

PDU *MsgHandle::MoveFile(PDU *pdu)
{
    qDebug()<<"case:MoveFile_REQUEST";
    int OldLen;
    int TarLen;
    memcpy(&OldLen,pdu->caData,sizeof (int));
    memcpy(&TarLen,pdu->caData+32,sizeof (int));
    char* OldPath = new char[OldLen +1];
    char* TarPath = new char[TarLen +1];
    memset(OldPath, 0 ,OldLen+1);
    memset(TarPath, 0 ,TarLen+1);
    memcpy(OldPath,pdu->caMsg,OldLen);
    memcpy(TarPath,pdu->caMsg + OldLen ,TarLen);
    bool ret = QFile::rename(OldPath,TarPath);
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;

}

PDU *MsgHandle::UploadFile(PDU *pdu)
{
    qDebug()<<"case:UploadFile_REQUEST";
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
    int ret = 0;
    if(m_bUpload){
        qDebug()<<"m_bUpload is true";
        ret = 1;
        memcpy(respdu->caData,&ret,sizeof(qint64));
    }
    char FileName[32] = {'\0'};
    qint64 Filesize = 0;
    memcpy(FileName,pdu->caData,32);
    memcpy(&Filesize,pdu->caData+32,sizeof (qint64));
    QString strPath = QString("%1/%2").arg(pdu->caMsg).arg(FileName);

    m_UploadFile.setFileName(strPath);
    if(m_UploadFile.open(QIODevice::WriteOnly)){
        m_bUpload = true;
        FilesTotalize = Filesize;
        FileReceivedsize = 0;
    }else{
        qDebug()<<"file open failed";
        ret = -1;
    }
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandle::UploadData(PDU *pdu)
{
    qDebug()<<"case:UploadData_REQUEST";
    m_UploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    FileReceivedsize += pdu->uiMsgLen;
    if(FileReceivedsize < FilesTotalize){
        return NULL;
    }
    m_bUpload = false;
    bool ret = FileReceivedsize == FilesTotalize;
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_DATA_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    m_UploadFile.close();
    return respdu;
}

PDU *MsgHandle::DownloadFile(PDU *pdu)
{
    qDebug()<<"case:DownloadFile_REQUEST";
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
    int ret = 0;
    if(m_bDownload){
        qDebug()<<"m_bDownload is true";
        ret = 1;
        memcpy(respdu->caData,&ret,sizeof(qint64));
    }
    char FileName[32] = {'\0'};
    memcpy(FileName,pdu->caData,32);
    DownloadPath = QString("%1/%2").arg(pdu->caMsg).arg(FileName);
    qint64 Filesize;
    m_DownloadFile.setFileName(DownloadPath);
    qDebug()<<DownloadPath;
    if(m_DownloadFile.open(QIODevice::ReadOnly)){
        qDebug()<<"file open true";
        m_bDownload = true;
        Filesize = m_DownloadFile.size();
    }else{
        qDebug()<<"file open failed";
        ret = -1;
    }
    memcpy(respdu->caData,&Filesize,sizeof(qint64));
    memcpy(respdu->caData + 32,&ret,sizeof(int));
    return respdu;
}

PDU *MsgHandle::DownloadData(PDU *pdu, MyTcpSocket *m_tcpSocket)
{
    qDebug()<<"case:DownloadData_REQUEST";
    bool mode;
   // char userName[32] = {'\0'};
    //memcpy(&userName,m_strLoginname.toStdString().c_str(),m_strLoginname.toStdString().size());
    memcpy(&mode,pdu->caData,sizeof(bool));
    //qDebug()<<"userName:  "<<userName<<" "<<mode;
    if(mode){
        PDU* datapdu = mkPDU(4096);
        datapdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_DATA_RESPOND;
        qint64 ret = 0;
        while(true){
            ret = m_DownloadFile.read(datapdu->caMsg,4096);
            if(ret <= 0){
                break;
            }
            qDebug()<<datapdu->caMsg;
            datapdu->uiMsgLen = ret;
            datapdu->uiPDULen = ret + sizeof (PDU);

            m_tcpSocket->write((char*)datapdu,datapdu->uiPDULen);
            //MyTcpServer::getInstance().resend(userName,datapdu);
        }
        free(datapdu);
        datapdu = NULL;
    }
    m_bDownload = false;
    m_DownloadFile.close();
    return NULL;
}

PDU *MsgHandle::ShareFile(PDU *pdu)
{
    qDebug()<<"case:ShareFile_REQUEST";
    //取出转发文件路径，转发用户数量
    int usernum;
    memcpy(&usernum,pdu->caData + 32,sizeof (int));
    qDebug()<<usernum;
    QString FilePath  = pdu->caMsg + (usernum * 32);
    qDebug()<<pdu->uiMsgLen - (usernum * 32);
    qDebug()<<FilePath;

    //构建转发Tarpdu ，caMsg中存储转发文件路径，caData中存储分享发送者名字
    PDU* Tarpdu = mkPDU(FilePath.toStdString().size());
    Tarpdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(Tarpdu->caData,pdu->caData,32);
    memcpy(Tarpdu->caMsg,FilePath.toStdString().c_str(),FilePath.toStdString().size());
    char TarName[32] = {'\0'};
    //转发
    for(int i = 0;i < usernum;i++){
        memcpy(TarName,pdu->caMsg + i*32,32);
        MyTcpServer::getInstance().resend(TarName,Tarpdu);
    }

    free(Tarpdu);
    Tarpdu = NULL;
    //构建回复pdu
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
    return respdu;
}

PDU *MsgHandle::ShareFileAgree(PDU *pdu)
{

    int sendFile_size;
    char sendFileName[32] = {'\0'};
    memcpy(sendFileName,pdu->caData,32);
    memcpy(&sendFile_size,pdu->caData + 32,sizeof (int));
    QString sendPath = pdu->caMsg;
    QString savePath = pdu->caMsg + sendFile_size ;
    qDebug()<<sendPath<<endl<<savePath;
    bool ret = QFile::copy(sendPath,QString("%1/%2").arg(savePath).arg(sendFileName));
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
    return NULL;
}
