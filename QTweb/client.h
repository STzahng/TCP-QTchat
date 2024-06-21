#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "protocol.h"
#include "reshandle.h"
QT_BEGIN_NAMESPACE
namespace Ui { class client; }
QT_END_NAMESPACE

class client : public QWidget
{
    Q_OBJECT

public:
    static client& getInstance();
    ~client();
    PDU* readPDU();
    void sendPDU(PDU* pdu);
    void handleMeg(PDU* pdu);
    QString getLoginName();
    ResHandle *m_Msghandle;//创建消息处理的对象

    QTcpSocket& getTcpSocket();

private slots:
//    void on_send_PB_clicked();


    void on_regist_PB_clicked();
    void recvMsg();
    void on_login_PB_clicked();

private:
    //实现单例模式，私有化构造函数，删除拷贝构造函数和拷贝赋值运算符
    client(QWidget*parent = nullptr);
    client(const client& instance) = delete ;
    client& operator=(const client&) = delete ;

    Ui::client *ui;
    //客户端ip和端口号
    QString m_strIP ;
    quint16 m_strPort ;
    void config_ip();
    QTcpSocket m_TcpSocket;
    QString m_StrLoginName;
};
#endif // CLIENT_H
