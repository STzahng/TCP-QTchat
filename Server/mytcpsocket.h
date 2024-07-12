#ifndef MTTCPSOCKET_H
#define MTTCPSOCKET_H
#include "protocol.h"
#include "msghandle.h"
#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    QString m_strLoginname;
    MsgHandle *m_Msghandle;
    PDU* readpdu();
    PDU* handleMsg(PDU* pdu);
    void sendPDU(PDU* pdu);

private:
    QByteArray buffer;

public slots:
    void recvMsg();//接收并处理消息
    void clientOffline();
signals:
    void Offline(MyTcpSocket *mySocket);
};

#endif // MTTCPSOCKET_H
