#include "onlineuser.h"
#include "ui_onlineuser.h"

#include <QString>
OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::showOnlineUser(QStringList userList)
{
    ui->OnlineList->clear();
    ui->OnlineList->addItems(userList);
}

void OnlineUser::on_OnlineList_itemDoubleClicked(QListWidgetItem *item)
{
    QString StrCurName = client::getInstance().getLoginName();
    QString StrTarName = item->text();
    PDU* pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData,StrCurName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,StrTarName.toStdString().c_str(),32);
    client::getInstance().sendPDU(pdu);
}
