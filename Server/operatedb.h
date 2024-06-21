#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
class OperateDb : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase m_db;//数据库对象
    static OperateDb& getInstance();
    void connect();//连接数据库
    bool handleRegist(char* name ,char* pwd);//注册
    bool handleLogin(char* name ,char* pwd);//登录
    void handleOffline(const char* name);//客户端online置为0
    int handleFindUser(const char* name);
    int handleAddFriend(const char* Cur,const char* Tar);
    void handleAddFriendAgree(const char* Cur,const char* Tar);
    int handleDelFriend(const char* Cur,const char* Tar);
    QStringList handleOnlineUser();
    QStringList handleFlushFriend(const char* caName);
    ~OperateDb();

private:
    explicit OperateDb(QObject *parent = nullptr);
    OperateDb(const OperateDb& instance) = delete ;
    OperateDb& operator=(const OperateDb&) = delete ;
signals:

};

#endif // OPERATEDB_H
