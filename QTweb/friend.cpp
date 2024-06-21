#include "friend.h"
#include "ui_friend.h"

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

Friend::~Friend()
{
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
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    client::getInstance().sendPDU(pdu);
}

void Friend::on_delFriend_PB_clicked()
{
    QListWidgetItem* pItem = ui->friend_List->currentItem();
    char strTarName[32] = {'\0'} ;
    if(!pItem){
        QMessageBox::information(this,"删除好友","请选择你要删除的好友");
        return;
    }else{
        memcpy(strTarName,pItem->text().toStdString().c_str(),32);
        int ret = QMessageBox::question(this,"删除好友",QString("确认删除好友 '%1'").arg(strTarName));
        if(ret != QMessageBox::Yes){
            return ;
        }
    }
    PDU* pdu =mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FRIEND_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    memcpy(pdu->caData+32,strTarName,32);
    client::getInstance().sendPDU(pdu);
    qDebug()<<"删除好友： "<<strTarName;
}
