#ifndef READER_H
#define READER_H

#include <QObject>

class reader : public QObject
{
    Q_OBJECT
public:
    explicit reader(QObject *parent = nullptr);
    void slot_newspaper();
    void slot_newspaper(QString str);
signals:

};

#endif // READER_H
