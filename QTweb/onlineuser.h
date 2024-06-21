#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QListWidget>
#include <QWidget>
#include "client.h"
#include "protocol.h"
namespace Ui {
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUser(QWidget *parent = nullptr);
    ~OnlineUser();
    void showOnlineUser(QStringList userList);

private slots:
    void on_OnlineList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::OnlineUser *ui;
};

#endif // ONLINEUSER_H
