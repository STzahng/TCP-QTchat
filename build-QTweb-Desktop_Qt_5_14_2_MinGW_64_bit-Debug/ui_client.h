/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *regist_PB;
    QSpacerItem *horizontalSpacer;
    QPushButton *login_PB;
    QFormLayout *formLayout;
    QLineEdit *username_LE;
    QLabel *label_2;
    QLineEdit *pwd_LE;
    QLabel *label;

    void setupUi(QWidget *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QString::fromUtf8("client"));
        client->resize(357, 209);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(14);
        client->setFont(font);
        gridLayout_2 = new QGridLayout(client);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        regist_PB = new QPushButton(client);
        regist_PB->setObjectName(QString::fromUtf8("regist_PB"));

        horizontalLayout->addWidget(regist_PB);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        login_PB = new QPushButton(client);
        login_PB->setObjectName(QString::fromUtf8("login_PB"));

        horizontalLayout->addWidget(login_PB);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        username_LE = new QLineEdit(client);
        username_LE->setObjectName(QString::fromUtf8("username_LE"));

        formLayout->setWidget(0, QFormLayout::FieldRole, username_LE);

        label_2 = new QLabel(client);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(16);
        label_2->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        pwd_LE = new QLineEdit(client);
        pwd_LE->setObjectName(QString::fromUtf8("pwd_LE"));
        pwd_LE->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, pwd_LE);

        label = new QLabel(client);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font2;
        font2.setPointSize(16);
        label->setFont(font2);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(client);

        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QWidget *client)
    {
        client->setWindowTitle(QCoreApplication::translate("client", "client", nullptr));
        regist_PB->setText(QCoreApplication::translate("client", "\346\263\250\345\206\214", nullptr));
        login_PB->setText(QCoreApplication::translate("client", "\347\231\273\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("client", "\345\257\206  \347\240\201\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("client", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
