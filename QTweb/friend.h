#ifndef FRIEND_H
#define FRIEND_H

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
    void showFriend(QStringList userList);
    OnlineUser* getOnlineUser();
    QStringList& getFriendList();
    ~Friend();

private slots:
    void on_findUser_PB_clicked();
    void on_onlineUser_PB_clicked();
    void on_flushFriend_PB_clicked();
    void on_delFriend_PB_clicked();

private:
    OnlineUser* m_pOnlineUser;
    QStringList FriendList;
    Ui::Friend *ui;
};

#endif // FRIEND_H
