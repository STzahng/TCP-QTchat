#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QWidget>
#include <protocol.h>
#include <qlistwidget.h>
#include "sharefile.h"
namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    QString strCurPath;
    QString strUserPath;
    QList<FileInfo*>m_FileInfoList;
    ShareFile* m_ShareFile;
    QString strShareFile;

    void flushFile();
    void updatFileList(QList<FileInfo*>pFileInfoList);
    void UploadFile();
    void DownLoadFile(qint64 Filesize);
    void DownLoadData(PDU* Datapdu);
signals:
    void emit_FlushFile();

private slots:
    void on_mkDir_PB_clicked();

    void on_flushFile_PB_clicked();

    void on_delDir_PB_clicked();

    void on_delFile_PB_clicked();

    void on_rename_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_moveFile_PB_clicked();

    void on_upload_PB_clicked();

    void on_download_PB_clicked();

    void on_shareFile_PB_clicked();

private:
    Ui::File *ui;

    QString MvFileName;
    QString MvOldPath;
    QString MvTarPath;

    bool m_bUpload;
    QString uploadPath;

    bool m_bdownload;
    qint64 FilesTotalize;
    qint64 FileReceivedsize;
    QFile m_DownloadFile;
    QString downloadPath;

};

#endif // FILE_H
