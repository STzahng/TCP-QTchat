#ifndef FRIEND_H
#define FRIEND_H

#include "chat.h"
#include "onlineuser.h"
#include <QWidget>

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    void flushFriend();
    void showFriend(QStringList userList);
    OnlineUser* getOnlineUser();
    QStringList& getFriendList();
    Chat* CreatChat(QString Name);
    void DelChat(Chat* Chatpage);
    ~Friend();

signals:
    void emit_FlushFriend();

private slots:
    void on_findUser_PB_clicked();
    void on_onlineUser_PB_clicked();
    void on_flushFriend_PB_clicked();
    void on_delFriend_PB_clicked();
    void on_chat_PB_clicked();

    void on_friend_List_itemDoubleClicked(QListWidgetItem *item);

private:
    QList<Chat*>m_ChatList;
    OnlineUser* m_pOnlineUser;

    QStringList FriendList;
    Ui::Friend *ui;
};

#endif // FRIEND_H
