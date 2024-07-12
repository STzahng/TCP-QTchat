#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    QString StrChatName;
    explicit Chat(QWidget *parent = nullptr,QString O_Name = "");
    void closeEvent(QCloseEvent *event) ;
    void updateshow_LE(QString SendUser,QString strMsg);
    ~Chat();

private slots:
    void on_send_PB_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
