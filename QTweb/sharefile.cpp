 #include "sharefile.h"
#include "ui_sharefile.h"
#include "client.h"
#include "index.h"
ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("分享文件"));
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::UpdataShareList()
{
    ui->FriendList->clear();
    QStringList FriendList = Index::getinstance().getFriend()->getFriendList();
    ui->FriendList->addItems(FriendList);
}

void ShareFile::on_allSelect_PB_clicked()
{
    for(int i = 0;i < ui->FriendList->count();i++){
        ui->FriendList->item(i)->setSelected(true);
    }
}

void ShareFile::on_canceSelect_PB_clicked()
{
    for(int i = 0;i < ui->FriendList->count();i++){
        ui->FriendList->item(i)->setSelected(false);
    }
}

void ShareFile::on_Ok_PB_clicked()
{
    //记录分享文件路径
    QString FilePath = QString("%1/%2")
            .arg(Index::getinstance().getFile()->strCurPath)
            .arg(Index::getinstance().getFile()->strShareFile);
    qDebug()<<FilePath;
    //获取选中的目标用户
    QList<QListWidgetItem*> pItems = ui->FriendList->selectedItems();
    int usernum = pItems.size();
    qDebug()<<usernum;
    //将目标用户名全部存在caMsg中，以及分享文件路径
    PDU* pdu = mkPDU(FilePath.toStdString().size() + usernum * 32 + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(pdu->caData,client::getInstance().getLoginName().toStdString().c_str(),32);
    memcpy(pdu->caData + 32,&usernum,sizeof(int));
    qDebug()<<*(int*)(pdu->caData +32);
    for(int i = 0;i < usernum ;i++){
        memcpy(pdu->caMsg + i * 32,pItems.at(i)->text().toStdString().c_str(),32);
    }
    memcpy(pdu->caMsg + usernum*32,FilePath.toStdString().c_str(),FilePath.toStdString().size());
    qDebug()<<pdu->caMsg + usernum*32;
    qDebug()<<pdu->uiMsgLen - (usernum * 32);
    qDebug()<<FilePath.toStdString().size();
    client::getInstance().sendPDU(pdu);
}
