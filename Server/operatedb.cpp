#include "operatedb.h"
#include "mytcpsocket.h"
#include <QDebug>
#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QString>

OperateDb::OperateDb(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

OperateDb::~OperateDb()
{
    m_db.close();
}

OperateDb &OperateDb::getInstance()
{
    static OperateDb instance;
    return instance;
}

void OperateDb::connect()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("qtweb");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("");
    if(m_db.open()){
        qDebug()<<"数据库连接成功...";
    }else {
        QMessageBox::critical(0,"连接数据库",m_db.lastError().text());
    }
}

bool OperateDb::handleRegist(char *name, char *pwd)
{
    qDebug()<<"sql_handleRegist";
    if(name == NULL||pwd == NULL){
        return false;
    }
    //检查添加的用户是否存在
    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    QSqlQuery q;

    if(!q.exec(sql) || q.next()){//q.next()返回上一次exec查询的结果，如果为空则代表创建的用户没有重复的
        qDebug()<<"添加用户失败"<<endl;
        return false;
    }

    //添加用户
    sql = QString("insert into user_info(name,pwd) values ('%1','%2')").arg(name).arg(pwd);
    qDebug()<<"注册用户成功..."<<endl
            <<"name: "<<name <<" "<<"pwd: "<<pwd;
    return q.exec(sql);
}

bool OperateDb::handleLogin(char *name, char *pwd)
{
    qDebug()<<"sql_handleLogin";
    if(name == NULL||pwd == NULL){
        return false;
    }
    //检查登录的用户是否存在
    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    QSqlQuery q;
    if(!q.exec(sql) || !q.next()){//q.next()返回上一次exec查询的结果，如果为空则代表创建的用户没有重复的
        qDebug()<<"登录失败"<<endl;
        return false;
    }
    //将用户online设置为1
    sql = QString("UPDATE user_info set online = 1 WHERE name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    qDebug()<<"用户登录成功..."<<endl
            <<"name: "<<name <<" "<<"pwd: "<<pwd;
    return q.exec(sql);
}

void OperateDb::handleOffline(const char* name)
{
    if(name == NULL){
        qDebug()<<"handleoffline name is NULL";
        return;
    }
    QString sql = QString("UPDATE user_info set online = 0 WHERE name = '%1'").arg(name);
    QSqlQuery q;
    qDebug()<<"将online设置为0  "<<name;
    q.exec(sql);
}

int OperateDb::handleFindUser(const char *name)
{
    if(name == NULL){
        qDebug()<<"handleoffline name is NULL";
        return -1;
    }
    QString sql = QString("SELECT online from user_info where name = '%1'").arg(name);
    QSqlQuery q;
    qDebug()<<"查找用户的online： "<<name;
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();
    }
    return -1;
}

int OperateDb::handleAddFriend(const char *Cur, const char *Tar)
{
    if(Cur == NULL||Tar == NULL){
        qDebug()<<"handleAddFriend is NULL";
        return -1;
    }
    QString sql = QString(R"(SELECT * from friend where
                          ( user_id = (SELECT id from user_info where name = '%1')
                          and friend_id = (SELECT id from user_info where name = '%2') )
                          or
                          ( user_id = (SELECT id from user_info where name = '%3')
                          and friend_id = (SELECT id from user_info where name = '%4'));
                          )").arg(Cur).arg(Tar).arg(Tar).arg(Cur);

    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return -2;//代表已经是好友
    }
    sql = QString("select online from user_info where name = '%1'").arg(Tar);
    q.exec(sql);
    if(q.next()){
        return q.value(0).toInt();//0不在线 1在线
    }
    return -1;
}

void OperateDb::handleAddFriendAgree(const char *Cur, const char *Tar)
{
    if(Cur == NULL||Tar == NULL){
        qDebug()<<"handleAddFriend is NULL";
        return ;
    }
    QString sql = QString(R"(INSERT INTO friend(user_id,friend_id)
                          SELECT u1.id,u2.id FROM user_info u1,user_info u2
                          WHERE u1.name = '%1'and u2.name = '%2';
                          )").arg(Cur).arg(Tar);

    QSqlQuery q;
    q.exec(sql);
}

int OperateDb::handleDelFriend(const char *Cur, const char *Tar)
{
    if(Cur == NULL||Tar == NULL){
        qDebug()<<"handleDelFriend is NULL";
        return 0;//
    }
    QSqlQuery q;
    QString sql = QString(R"(SELECT * from friend where
                          ( user_id = (SELECT id from user_info where name = '%1')
                          and friend_id = (SELECT id from user_info where name = '%2') )
                          or
                          ( user_id = (SELECT id from user_info where name = '%3')
                          and friend_id = (SELECT id from user_info where name = '%4'));
                          )").arg(Cur).arg(Tar).arg(Tar).arg(Cur);
    q.exec(sql);
    if(!q.next()){
        return -2;
    }
    sql = QString(R"(DELETE from friend WHERE
                          ( user_id = (SELECT id from user_info where name = "%1")
                          and friend_id = (SELECT id from user_info where name = "%2") )
                          or
                          ( user_id = (SELECT id from user_info where name = "%3")
                          and friend_id = (SELECT id from user_info where name = "%4"));
                          )").arg(Cur).arg(Tar).arg(Tar).arg(Cur);
    return q.exec(sql);
}

QStringList OperateDb::handleOnlineUser()
{
    QString sql = QString("SELECT name from user_info where online = 1");
    QSqlQuery q;
    qDebug()<<"调用查找在线用户";
    q.exec(sql);
    QStringList res;
    while(q.next()){
        res.append(q.value(0).toString());
    }
    return res;
}

QStringList OperateDb::handleFlushFriend(const char *caName)
{
    QStringList res;
    if(caName == NULL){
        return res;
    }
    QString sql = QString(R"(SELECT name FROM user_info WHERE id in
                          (SELECT friend_id FROM friend where user_id = (SELECT id FROM user_info WHERE name = '%1')
                          UNION
                          SELECT user_id FROM friend where friend_id = (SELECT id FROM user_info WHERE name = '%1'))
                          and online = 1;)").arg(caName);
    QSqlQuery q;
    qDebug()<<"刷新好友";
    q.exec(sql);
    while(q.next()){
        res.append(q.value(0).toString());
    }
    return res;
}



