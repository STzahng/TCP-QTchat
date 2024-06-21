#include "client.h"
#include "index.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client::getInstance().show();

    return a.exec();
}
