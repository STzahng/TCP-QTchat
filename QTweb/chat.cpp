#include "chat.h"
#include "ui_chat.h"
#include "client.h"
#include "index.h"
#include "protocol.h"
#include "friend.h"
#include <QDebug>

Chat::Chat(QWidget *parent,QString O_Name) :
    QWidget(parent),StrChatName(O_Name),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    this->setWindowTitle(StrChatName);
}

void Chat::closeEvent(QCloseEvent *event)
{
    Index::getinstance().getFriend()->DelChat(this);
}

void Chat::updateshow_LE(QString SendUser,QString strMsg)
{
    if(SendUser == client::getInstance().getLoginName()){
        ui->show_TE->setAlignment(Qt::AlignRight);
    }else{
        ui->show_TE->setAlignment(Qt::AlignLeft);
    }
    ui->show_TE->append(strMsg);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_send_PB_clicked()
{
    QString ChatMsg = ui->input_LE->text();
    if(ChatMsg.isEmpty()){
        return;
    }
    //ChatMsg+='\n';
    PDU* pdu = mkPDU(ChatMsg.toStdString().size());
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    memcpy(pdu->caData+32,StrChatName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,ChatMsg.toStdString().c_str(),ChatMsg.toStdString().size());
    QString StrMsg = QString("%2: %1\n").arg(client::getInstance().getLoginName()).arg(pdu->caMsg);
    this->updateshow_LE(client::getInstance().getLoginName(),StrMsg);
    client::getInstance().sendPDU(pdu);
    ui->input_LE->clear();
}
