#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:

    ~Server();
    static Server& S_getinstance();
    QString getRootPath();
private:
    Ui::Server *ui;
    Server(QWidget *parent = nullptr);
    Server(const Server& S_instance) = delete ;
    Server& operator()(const Server&) = delete ;
    //获取服务器ip和端口号
    void config_ip();
    QString s_strIp;
    quint16 s_strPort;
    QString s_strRootPath;
};
#endif // SERVER_H
