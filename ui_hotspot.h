/********************************************************************************
** Form generated from reading UI file 'hotspot.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTSPOT_H
#define UI_HOTSPOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HotSpot
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget_Photo;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_changePhoto;
    QTextEdit *textEdit_Description;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_position;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_deleteHotSpot;

    void setupUi(QWidget *HotSpot)
    {
        if (HotSpot->objectName().isEmpty())
            HotSpot->setObjectName(QStringLiteral("HotSpot"));
        HotSpot->resize(418, 536);
        HotSpot->setMinimumSize(QSize(418, 536));
        HotSpot->setMaximumSize(QSize(418, 536));
        verticalLayout = new QVBoxLayout(HotSpot);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_Photo = new QWidget(HotSpot);
        widget_Photo->setObjectName(QStringLiteral("widget_Photo"));
        widget_Photo->setMinimumSize(QSize(400, 300));
        widget_Photo->setMaximumSize(QSize(400, 300));

        verticalLayout->addWidget(widget_Photo);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_changePhoto = new QPushButton(HotSpot);
        pushButton_changePhoto->setObjectName(QStringLiteral("pushButton_changePhoto"));

        horizontalLayout_2->addWidget(pushButton_changePhoto);


        verticalLayout->addLayout(horizontalLayout_2);

        textEdit_Description = new QTextEdit(HotSpot);
        textEdit_Description->setObjectName(QStringLiteral("textEdit_Description"));
        textEdit_Description->setMinimumSize(QSize(400, 130));
        textEdit_Description->setMaximumSize(QSize(400, 130));

        verticalLayout->addWidget(textEdit_Description);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_position = new QLabel(HotSpot);
        label_position->setObjectName(QStringLiteral("label_position"));
        label_position->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_position);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_deleteHotSpot = new QPushButton(HotSpot);
        pushButton_deleteHotSpot->setObjectName(QStringLiteral("pushButton_deleteHotSpot"));
        pushButton_deleteHotSpot->setMaximumSize(QSize(75, 16777215));

        horizontalLayout->addWidget(pushButton_deleteHotSpot);


        verticalLayout->addLayout(horizontalLayout);

        widget_Photo->raise();
        textEdit_Description->raise();
        label_position->raise();
        pushButton_changePhoto->raise();
        pushButton_deleteHotSpot->raise();

        retranslateUi(HotSpot);

        QMetaObject::connectSlotsByName(HotSpot);
    } // setupUi

    void retranslateUi(QWidget *HotSpot)
    {
        HotSpot->setWindowTitle(QApplication::translate("HotSpot", "Form", Q_NULLPTR));
        pushButton_changePhoto->setText(QApplication::translate("HotSpot", "\346\233\264\346\224\271\345\233\276\347\211\207", Q_NULLPTR));
        label_position->setText(QApplication::translate("HotSpot", "\345\235\220\346\240\207", Q_NULLPTR));
        pushButton_deleteHotSpot->setText(QApplication::translate("HotSpot", "\345\210\240\351\231\244\347\203\255\347\202\271", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HotSpot: public Ui_HotSpot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTSPOT_H
