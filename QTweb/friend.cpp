#include "friend.h"
#include "ui_friend.h"
#include "index.h"
#include "protocol.h"
#include "client.h"


#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_pOnlineUser = new OnlineUser;
    connect(this,&Friend::emit_FlushFriend,this,&Friend::on_flushFriend_PB_clicked);
    emit emit_FlushFriend();
}

void Friend::flushFriend()
{
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    client::getInstance().sendPDU(pdu);
}

void Friend::showFriend(QStringList userList)
{
    ui->friend_List->clear();
    ui->friend_List->addItems(userList);
}

OnlineUser *Friend::getOnlineUser()
{
    return m_pOnlineUser;
}

QStringList& Friend::getFriendList()
{
    return FriendList;
}

Chat* Friend::CreatChat(QString Name)
{
    foreach(Chat* pChat,m_ChatList){
        if(pChat->StrChatName == Name)return pChat;//防止创建多个已存在的聊天窗口
    }
    Chat* m_pChat = new Chat(nullptr,Name);
    m_ChatList.append(m_pChat);
    qDebug()<<m_pChat->StrChatName;
    if(m_pChat->isHidden()){
        m_pChat->show();
    }
    return m_pChat;
}

void Friend::DelChat(Chat *Chatpage)
{
    m_ChatList.removeOne(Chatpage);//移除链表中的关闭客户端
    Chatpage->deleteLater();//等待当前Socket任务结束后在释放
    Chatpage = NULL;

    //测试是否移除成功
//    foreach(Chat* pChat,m_ChatList){
//        qDebug()<<pChat->StrChatName;
//    }
}

Friend::~Friend()
{

    for(auto it:m_ChatList){
        delete it;
    }
    delete m_pOnlineUser;
    delete ui;
}

void Friend::on_findUser_PB_clicked()
{
    QString strName;
    while(true){
        bool ok;
        strName = QInputDialog::getText(this,"搜索","用户名：",QLineEdit::Normal, "", &ok);
        if(!ok){
            return;
        }
        if(strName == NULL){
            QMessageBox::critical(0,"搜索","用户名为空!");
        }else{
            qDebug()<<"friend_name: "<<strName;
            break;
        }
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_FIND_USER_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    client::getInstance().sendPDU(pdu);
}

void Friend::on_onlineUser_PB_clicked()
{
    if(m_pOnlineUser->isHidden()){
        m_pOnlineUser->show();
    }
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_ONLINE_USER_REQUEST;
    client::getInstance().sendPDU(pdu);
}

void Friend::on_flushFriend_PB_clicked()
{
    flushFriend();
}

void Friend::on_delFriend_PB_clicked()
{
    QListWidgetItem* pItem = ui->friend_List->currentItem();

    QString strTarName;
    if(!pItem){
        QMessageBox::information(this,"删除好友","请选择你要删除的好友");
        return;
    }else{
        strTarName = pItem->text() ;
        int ret = QMessageBox::question(this,"删除好友",QString("确认删除好友 '%1'").arg(strTarName));
        if(ret != QMessageBox::Yes){
            return ;
        }
    }
    PDU* pdu =mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName.toStdString().c_str(),32);
    client::getInstance().sendPDU(pdu);
    qDebug()<<"删除好友： "<<strTarName;
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem* pItem = ui->friend_List->currentItem();
    qDebug()<<"1";

    if(!pItem){
        qDebug()<<"3";
        QMessageBox::information(this,"聊天","未选择聊天好友");
        return;
    }else{
        QString TarName = QString(pItem->text());
        CreatChat(TarName);
    }
}

void Friend::on_friend_List_itemDoubleClicked(QListWidgetItem *item)
{
    QString TarName = QString(item->text());
    CreatChat(TarName);
}
