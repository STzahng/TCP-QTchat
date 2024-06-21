/********************************************************************************
** Form generated from reading UI file 'onlineuser.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINEUSER_H
#define UI_ONLINEUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OnlineUser
{
public:
    QGridLayout *gridLayout;
    QListWidget *OnlineList;

    void setupUi(QWidget *OnlineUser)
    {
        if (OnlineUser->objectName().isEmpty())
            OnlineUser->setObjectName(QString::fromUtf8("OnlineUser"));
        OnlineUser->resize(386, 370);
        gridLayout = new QGridLayout(OnlineUser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        OnlineList = new QListWidget(OnlineUser);
        OnlineList->setObjectName(QString::fromUtf8("OnlineList"));

        gridLayout->addWidget(OnlineList, 0, 0, 1, 1);


        retranslateUi(OnlineUser);

        QMetaObject::connectSlotsByName(OnlineUser);
    } // setupUi

    void retranslateUi(QWidget *OnlineUser)
    {
        OnlineUser->setWindowTitle(QCoreApplication::translate("OnlineUser", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OnlineUser: public Ui_OnlineUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINEUSER_H
