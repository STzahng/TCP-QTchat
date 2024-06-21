#ifndef NEWSPAPER_H
#define NEWSPAPER_H

#include <QObject>

class news_paper : public QObject
{
    Q_OBJECT
public:
    explicit news_paper(QObject *parent = nullptr);
    void send(QString str)
        {
            emit signal_newparer(str);
        }
    void send()
        {
            emit signal_newparer();
        }
signals:
    void signal_newparer(QString str);
    void signal_newparer();

};

#endif // NEWSPAPER_H
