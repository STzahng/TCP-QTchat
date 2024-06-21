#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QTcpServer>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle) ;
    void resend(char* TarName,PDU* pdu);

public slots:
    void deleteSocket(MyTcpSocket *mySocket);

private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& instance) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<MyTcpSocket*>m_tcpSocketList;
};

#endif // MYTCPSERVER_H
