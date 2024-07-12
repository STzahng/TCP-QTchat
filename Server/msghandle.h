#ifndef MSGHANDLE_H
#define MSGHANDLE_H
#include "protocol.h"
//#include "mytcpsocket.h"
#include <QFile>
#include <QString>

class MyTcpSocket;
class MsgHandle
{
public:
    MsgHandle();
    PDU* REGIST(PDU* pdu);
    PDU* LOGIN(PDU* pdu,QString& m_strLoginname);
    PDU* FindUser(PDU* pdu);
    PDU* OnlineUser(PDU* pdu);
    PDU* AddFriend(PDU* pdu);
    PDU* AddFriendAgree(PDU* pdu);
    PDU* FlushFriend(PDU* pdu);
    PDU* DelFriend(PDU* pdu);
    void Chat(PDU* pdu);
    PDU* MKDir(PDU* pdu);
    PDU* FlushFile(PDU* pdu);
    PDU* DelDir(PDU* pdu);
    PDU* DelFile(PDU* pdu);
    PDU* ReName(PDU* pdu);
    PDU* MoveFile(PDU* pdu);
    PDU* UploadFile(PDU* pdu);
    PDU* UploadData(PDU* pdu);
    PDU* DownloadFile(PDU* pdu);
    PDU* DownloadData(PDU* pdu,MyTcpSocket* m_tcpSocket);
    PDU* ShareFile(PDU* pdu);
    PDU* ShareFileAgree(PDU* pdu);

public:
    //MyTcpSockety* m_tcpSocket;
    bool m_bUpload;
    QFile m_UploadFile;
    qint64 FilesTotalize;
    qint64 FileReceivedsize;

    bool m_bDownload;
    QFile m_DownloadFile;
    QString DownloadPath;



};

#endif // MSGHANDLE_H
