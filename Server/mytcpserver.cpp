#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接...";
    MyTcpSocket *pTcpSocket = new MyTcpSocket;
    //将客户端的socket使用函数进行初始化pTcpSocket并存入链表m_tcpSocketList

    pTcpSocket->setSocketDescriptor(handle);
    m_tcpSocketList.append(pTcpSocket);
//    foreach(MyTcpSocket *pTcpSocket,m_tcpSocketList){
//        qDebug()<<pTcpSocket;
//    }
    //测试打印客户端链接链表
    connect(pTcpSocket,&MyTcpSocket::Offline,this,&MyTcpServer::deleteSocket);
}

void MyTcpServer::resend(char *TarName, PDU *pdu)
{
    if(TarName == NULL||pdu == NULL){
        return ;
    }
    for(int i= 0 ;i<m_tcpSocketList.size();i++){
        if(TarName == m_tcpSocketList.at(i)->m_strLoginname){
            m_tcpSocketList.at(i)->write((char*)pdu,pdu->uiPDULen);
        }
    }
}

void MyTcpServer::deleteSocket(MyTcpSocket *mySocket)
{
    m_tcpSocketList.removeOne(mySocket);//移除链表中的关闭客户端
    mySocket->deleteLater();//等待当前Socket任务结束后在释放
    mySocket = NULL;

    //测试是否移除成功
    foreach(MyTcpSocket* pSocket,m_tcpSocketList){
        qDebug()<<pSocket->m_strLoginname;
    }
}

