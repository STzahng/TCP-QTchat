#include "file.h"
#include "ui_file.h"
#include "client.h"
#include "index.h"
#include "protocol.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    strCurPath = QString("%1/%2").arg(client::getInstance().getCurPath()).arg(client::getInstance().getLoginName());
    strUserPath = strCurPath;
    m_ShareFile = new ShareFile();
    m_bUpload = false;
    m_bdownload = false;
    connect(this,&File::emit_FlushFile,this,&File::flushFile);
    emit emit_FlushFile();
}

File::~File()
{
    delete m_ShareFile;
    delete ui;

}

void File::flushFile()
{

    PDU* pdu = mkPDU(strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

void File::updatFileList(QList<FileInfo *> pFileInfoList)
{
    //释放并更新m_FileList
    qDebug()<<"\nfile: updatFileList";
    qDeleteAll(m_FileInfoList);
    m_FileInfoList.clear();//clear并会自动释放指针对象
    m_FileInfoList = pFileInfoList;

    //清空并更新列表框
    ui->listWidget->clear();
    foreach(FileInfo* pFileInfo,pFileInfoList){
        QListWidgetItem *pItem = new QListWidgetItem;
        if(pFileInfo->Filetype == 0){
            pItem->setIcon(QIcon(QPixmap(":/Dir.png")));
        }else{
            pItem->setIcon(QIcon(QPixmap(":/File.png")));
        }
        qDebug()<<pFileInfo->caName;
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);
    }

}

void File::UploadFile()
{
    QFile file(uploadPath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"上传文件","打开文件失败");
        return;
    }
    m_bUpload = true;
    PDU* datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_DATA_REQUEST;
    qint64 ret = 0;
    while(true){
        ret = file.read(datapdu->caMsg,4096);
        if(ret == 0){
            break;
        }
        if(ret < 0){
            QMessageBox::warning(this,"上传文件","上传文件失败：读取错误");
            break;
        }
        datapdu->uiMsgLen = ret;
        datapdu->uiPDULen = ret + sizeof (PDU);
        client::getInstance().getTcpSocket().write((char*)datapdu,datapdu->uiPDULen);
    }
    m_bUpload = false;
    file.close();
    free(datapdu);
    datapdu = NULL;
}

void File::DownLoadFile(qint64 Filesize)
{

    m_DownloadFile.setFileName(downloadPath);
    bool ret = m_DownloadFile.open(QIODevice::WriteOnly);
    if(ret){
        m_bdownload = true;
        FilesTotalize = Filesize;
        FileReceivedsize = 0;
    }else{
        QMessageBox::warning(this,"下载文件","下载文件失败：无法建立目标文件");
        m_bdownload = false;

    }
    PDU* respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_DATA_REQUEST;
    memcpy(respdu->caData,&ret,sizeof(bool));
    client::getInstance().sendPDU(respdu);

}

void File::DownLoadData(PDU *Datapdu)
{
    m_DownloadFile.write(Datapdu->caMsg,Datapdu->uiMsgLen);
         FileReceivedsize += Datapdu->uiMsgLen;
         if(FileReceivedsize < FilesTotalize){
             return;
         }
         m_bdownload = false;
         if(FileReceivedsize != FilesTotalize){
             QMessageBox::warning(this,"下载文件","下载文件失败：文件传输错误");
         }else{
             QMessageBox::information(this,"下载文件","下载文件完成");
         }
         m_DownloadFile.close();

}


void File::on_mkDir_PB_clicked()
{
   QString NewDir = QInputDialog::getText(this,"创建文件夹","文件夹名： ");
   if(NewDir.isEmpty()||NewDir.toStdString().size()>32){
       QMessageBox::warning(this,"创建文件夹","文件夹名不合法");
       return;
   }
   PDU* pdu = mkPDU(strCurPath.size()+1);
   pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;
   memcpy(pdu->caData,NewDir.toStdString().c_str(),32);
   memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.toStdString().size());
   client::getInstance().sendPDU(pdu);
}

void File::on_flushFile_PB_clicked()
{
    flushFile();
}

void File::on_delDir_PB_clicked()
{
    //判断是否为选中文件夹
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"删除文件","请选择要删除的文件夹");
        return;
    }
    //判断选中的是否为文件夹类型
    QString DirName = pItem->text();
    foreach(FileInfo* pfileInfo,m_FileInfoList){
        if(pfileInfo->caName == DirName && pfileInfo->Filetype!=0){
            QMessageBox::warning(this,"删除文件","您选择的不是文件夹");
            return;
        }
    }
    //确认删除
    int ret = QMessageBox::question(this,"删除文件",QString("是否确定删除文件夹： %1").arg(DirName));
    if(ret != QMessageBox::Yes){
        return;
    }
    //合并路径将其发送给服务器
    QString path = strCurPath + "/" + DirName;
    qDebug()<<path;
    PDU* pdu = mkPDU(path.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
    memcpy(pdu->caMsg,path.toStdString().c_str(),path.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

void File::on_delFile_PB_clicked()
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"删除文件","请选择要删除的文件");
        return;
    }
    QString FileName = pItem->text();
    foreach(FileInfo* pfileInfo,m_FileInfoList){
        if(pfileInfo->caName == FileName && pfileInfo->Filetype!=1){
            QMessageBox::warning(this,"删除文件","您选择的不是文件");
            return;
        }
    }
    int ret = QMessageBox::question(this,"删除文件",QString("是否确定删除文件： %1").arg(FileName));
    if(ret != QMessageBox::Yes){
        return;
    }
    QString path = strCurPath + "/" + FileName;
    qDebug()<<path;
    PDU* pdu = mkPDU(path.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_REQUEST;
    memcpy(pdu->caMsg,path.toStdString().c_str(),path.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

void File::on_rename_PB_clicked()
{
    //判断是否选中
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"重命名文件","请选择要重命名的文件");
        return;
    }
    //获取原始文件名并输入修改后的文件名
    QString OldName = pItem->text();
    QString NewName = QInputDialog::getText(this,"重命名文件","文件名： ");
    if(NewName.isEmpty() || NewName.toStdString().size() > 32){
        QMessageBox::warning(this,"重命名文件","文件名非法");
        return;
    }
    PDU* pdu = mkPDU(strCurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_REQUEST;
    memcpy(pdu->caData,OldName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,NewName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString DirName = item->text();
    foreach(FileInfo* pfileInfo,m_FileInfoList){
        if(pfileInfo->caName == DirName && pfileInfo->Filetype!=0){
            return;
        }
    }
    strCurPath = strCurPath + "/" + DirName;
    flushFile();
}

void File::on_return_PB_clicked()
{
    if(strCurPath == strUserPath){
        QMessageBox::warning(this,"提示","无法返回上一级");
        return;
    }
    int index = strCurPath.lastIndexOf('/');
    strCurPath.remove(index,strCurPath.size()-index);
    flushFile();
}

void File::on_moveFile_PB_clicked()
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(ui->moveFile_PB->text() == "移动文件"){
        if(pItem == NULL){
            QMessageBox::warning(this,"移动文件","请选择要移动的文件");
            return;
        }
        QMessageBox::information(this,"移动文件","请选择移动的位置");
        ui->moveFile_PB->setText("确认/取消") ;
        MvFileName = pItem->text();
        MvOldPath = strCurPath + "/" + MvFileName;

    }else{
        QString msgBox;
        if(pItem == NULL){
            msgBox = QString("请确认是否将文件移至当前目录下");
            MvTarPath = strCurPath + "/" +MvFileName;
        }else{
            QString DirName = pItem->text();
            foreach(FileInfo* pfileInfo,m_FileInfoList){
                if(pfileInfo->caName == DirName && pfileInfo->Filetype != 0){
                    QMessageBox::warning(this,"移动文件","当前选中的不是文件夹，请重新选择");
                    return;
                }
            }
            MvTarPath = strCurPath + "/" + DirName + "/" + MvFileName;
            msgBox = QString("请确认将文件移至 %1").arg(DirName);
        }
        int ret = QMessageBox::question(this,"移动文件",msgBox,"确认","取消");
        ui->moveFile_PB->setText("移动文件");
        if(ret != 0){
            return;
        }
        int OldLen = MvOldPath.toStdString().size();
        int TarLen = MvTarPath.toStdString().size();
        PDU* pdu = mkPDU(OldLen+TarLen + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
        memcpy(pdu->caData,&OldLen,sizeof(int));
        memcpy(pdu->caData + 32,&TarLen,sizeof(int));
        memcpy(pdu->caMsg,MvOldPath.toStdString().c_str(),OldLen);
        memcpy(pdu->caMsg + OldLen,MvTarPath.toStdString().c_str(),TarLen);
        client::getInstance().sendPDU(pdu);

    }
}

void File::on_upload_PB_clicked()
{
    if(m_bUpload){
        QMessageBox::warning(this,"上传文件","已有文件正在上传，请稍等。");
        return;
    }
    uploadPath = QFileDialog::getOpenFileName();
    int index = uploadPath.lastIndexOf('/');
    QString FileName = uploadPath.right(uploadPath.size() - index - 1);
    QFile file(uploadPath);
    qint64 FileSize = file.size();
    PDU* pdu = mkPDU(strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memcpy(pdu->caData,FileName.toStdString().c_str(),32);
    memcpy(pdu->caData + 32,&FileSize,sizeof(qint64));
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.toStdString().size());

    client::getInstance().sendPDU(pdu);

}

void File::on_download_PB_clicked()
{
    if(m_bdownload){
        QMessageBox::warning(this,"下载文件","已有文件正在下载，请稍等。");
        return ;
    }
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"下载文件","请选择要下载的文件");
        return;
    }
    QString FileName = pItem->text();
    foreach(FileInfo* pfileInfo,m_FileInfoList){
        if(pfileInfo->caName == FileName && pfileInfo->Filetype!=1){
            QMessageBox::warning(this,"下载文件","您选择的不是文件");
            return;
        }
    }
    downloadPath.clear();
    downloadPath = QFileDialog::getSaveFileName();
    qDebug()<<downloadPath;
    PDU* pdu = mkPDU(strCurPath.toStdString().size());
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->caData,FileName.toStdString().c_str(),FileName.toStdString().size());
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

void File::on_shareFile_PB_clicked()
{
    Index::getinstance().getFriend()->flushFriend();
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(pItem == NULL){
        QMessageBox::warning(this,"分享文件","请选择要分享的文件");
        return;
    }
    strShareFile = pItem->text();
    qDebug()<<strShareFile;
    m_ShareFile->UpdataShareList();
    if(m_ShareFile->isHidden()){
        m_ShareFile->show();
    }
}
