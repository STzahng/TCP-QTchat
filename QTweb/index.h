#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include "friend.h"
#include "file.h"
namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    static Index& getinstance();
    ~Index();
    Friend* getFriend();
    File* getFile();

private slots:

    void on_friend_PB_clicked();

    void on_file_PB_clicked();


private:
    explicit Index(QWidget *parent = nullptr);
    Index(const Index& instance) = delete ;
    Index& operator=(const Index&) = delete;
    Ui::Index *ui;
};

#endif // INDEX_H
