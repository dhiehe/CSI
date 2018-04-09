/********************************************************************************
** Form generated from reading UI file 'scenecontrollerwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCENECONTROLLERWINDOW_H
#define UI_SCENECONTROLLERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "airphoto.h"

QT_BEGIN_NAMESPACE

class Ui_SceneControllerWindow
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_measureDistance;
    QGridLayout *gridLayout;
    QLabel *label_pointA;
    QLabel *label_pointAValue;
    QLabel *label_pointB;
    QLabel *label_pointBValue;
    QLabel *label_distanceAB;
    QLabel *label_distanceABValue;
    QGroupBox *groupBox_showButtons;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_showModel;
    QPushButton *pushButton_showCoverBox;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_showOctreeBoxs;
    QPushButton *pushButton_showRoamPath;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_showCoordinate;
    QPushButton *pushButton_Empty;
    QGroupBox *groupBox_changeCoverBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_sliderX;
    QSlider *sliderX1;
    QSlider *sliderX2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_sliderY;
    QSlider *sliderY1;
    QSlider *sliderY2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_silderZ;
    QSlider *sliderZ1;
    QSlider *sliderZ2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_4;
    AirPhoto *widget_AirPhoto;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_roamClear;
    QPushButton *pushButton_roamStart;
    QLabel *label_currentPosition;
    QLabel *label_currentPositionValue;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *SceneControllerWindow)
    {
        if (SceneControllerWindow->objectName().isEmpty())
            SceneControllerWindow->setObjectName(QStringLiteral("SceneControllerWindow"));
        SceneControllerWindow->resize(684, 359);
        SceneControllerWindow->setMinimumSize(QSize(614, 359));
        SceneControllerWindow->setMaximumSize(QSize(10000, 365));
        horizontalLayout_9 = new QHBoxLayout(SceneControllerWindow);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_measureDistance = new QGroupBox(SceneControllerWindow);
        groupBox_measureDistance->setObjectName(QStringLiteral("groupBox_measureDistance"));
        groupBox_measureDistance->setMinimumSize(QSize(0, 0));
        groupBox_measureDistance->setMaximumSize(QSize(206, 16777215));
        gridLayout = new QGridLayout(groupBox_measureDistance);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_pointA = new QLabel(groupBox_measureDistance);
        label_pointA->setObjectName(QStringLiteral("label_pointA"));
        label_pointA->setMinimumSize(QSize(20, 20));
        label_pointA->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(label_pointA, 0, 0, 1, 1);

        label_pointAValue = new QLabel(groupBox_measureDistance);
        label_pointAValue->setObjectName(QStringLiteral("label_pointAValue"));
        label_pointAValue->setMinimumSize(QSize(150, 20));
        label_pointAValue->setMaximumSize(QSize(150, 20));

        gridLayout->addWidget(label_pointAValue, 0, 1, 1, 1);

        label_pointB = new QLabel(groupBox_measureDistance);
        label_pointB->setObjectName(QStringLiteral("label_pointB"));
        label_pointB->setMinimumSize(QSize(30, 20));
        label_pointB->setMaximumSize(QSize(30, 20));

        gridLayout->addWidget(label_pointB, 1, 0, 1, 1);

        label_pointBValue = new QLabel(groupBox_measureDistance);
        label_pointBValue->setObjectName(QStringLiteral("label_pointBValue"));
        label_pointBValue->setMinimumSize(QSize(150, 20));
        label_pointBValue->setMaximumSize(QSize(150, 20));

        gridLayout->addWidget(label_pointBValue, 1, 1, 1, 1);

        label_distanceAB = new QLabel(groupBox_measureDistance);
        label_distanceAB->setObjectName(QStringLiteral("label_distanceAB"));
        label_distanceAB->setMinimumSize(QSize(60, 20));
        label_distanceAB->setMaximumSize(QSize(60, 20));

        gridLayout->addWidget(label_distanceAB, 2, 0, 1, 1);

        label_distanceABValue = new QLabel(groupBox_measureDistance);
        label_distanceABValue->setObjectName(QStringLiteral("label_distanceABValue"));
        label_distanceABValue->setMinimumSize(QSize(100, 20));
        label_distanceABValue->setMaximumSize(QSize(100, 20));

        gridLayout->addWidget(label_distanceABValue, 2, 1, 1, 1);


        verticalLayout_3->addWidget(groupBox_measureDistance);

        groupBox_showButtons = new QGroupBox(SceneControllerWindow);
        groupBox_showButtons->setObjectName(QStringLiteral("groupBox_showButtons"));
        groupBox_showButtons->setMaximumSize(QSize(206, 16777215));
        verticalLayout_2 = new QVBoxLayout(groupBox_showButtons);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_showModel = new QPushButton(groupBox_showButtons);
        pushButton_showModel->setObjectName(QStringLiteral("pushButton_showModel"));
        pushButton_showModel->setMinimumSize(QSize(75, 23));
        pushButton_showModel->setMaximumSize(QSize(75, 23));

        horizontalLayout_4->addWidget(pushButton_showModel);

        pushButton_showCoverBox = new QPushButton(groupBox_showButtons);
        pushButton_showCoverBox->setObjectName(QStringLiteral("pushButton_showCoverBox"));
        pushButton_showCoverBox->setMinimumSize(QSize(75, 23));
        pushButton_showCoverBox->setMaximumSize(QSize(75, 23));

        horizontalLayout_4->addWidget(pushButton_showCoverBox);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_showOctreeBoxs = new QPushButton(groupBox_showButtons);
        pushButton_showOctreeBoxs->setObjectName(QStringLiteral("pushButton_showOctreeBoxs"));
        pushButton_showOctreeBoxs->setMinimumSize(QSize(75, 23));
        pushButton_showOctreeBoxs->setMaximumSize(QSize(75, 23));

        horizontalLayout_5->addWidget(pushButton_showOctreeBoxs);

        pushButton_showRoamPath = new QPushButton(groupBox_showButtons);
        pushButton_showRoamPath->setObjectName(QStringLiteral("pushButton_showRoamPath"));
        pushButton_showRoamPath->setEnabled(false);
        pushButton_showRoamPath->setMinimumSize(QSize(75, 23));
        pushButton_showRoamPath->setMaximumSize(QSize(75, 23));

        horizontalLayout_5->addWidget(pushButton_showRoamPath);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        pushButton_showCoordinate = new QPushButton(groupBox_showButtons);
        pushButton_showCoordinate->setObjectName(QStringLiteral("pushButton_showCoordinate"));
        pushButton_showCoordinate->setMinimumSize(QSize(75, 23));
        pushButton_showCoordinate->setMaximumSize(QSize(75, 23));

        horizontalLayout_8->addWidget(pushButton_showCoordinate);

        pushButton_Empty = new QPushButton(groupBox_showButtons);
        pushButton_Empty->setObjectName(QStringLiteral("pushButton_Empty"));
        pushButton_Empty->setEnabled(true);
        pushButton_Empty->setMinimumSize(QSize(75, 23));
        pushButton_Empty->setMaximumSize(QSize(75, 23));

        horizontalLayout_8->addWidget(pushButton_Empty);


        verticalLayout_2->addLayout(horizontalLayout_8);


        verticalLayout_3->addWidget(groupBox_showButtons);

        groupBox_changeCoverBox = new QGroupBox(SceneControllerWindow);
        groupBox_changeCoverBox->setObjectName(QStringLiteral("groupBox_changeCoverBox"));
        groupBox_changeCoverBox->setMaximumSize(QSize(206, 16777215));
        verticalLayout_4 = new QVBoxLayout(groupBox_changeCoverBox);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_sliderX = new QLabel(groupBox_changeCoverBox);
        label_sliderX->setObjectName(QStringLiteral("label_sliderX"));
        label_sliderX->setMinimumSize(QSize(21, 16));
        label_sliderX->setMaximumSize(QSize(21, 16));

        horizontalLayout->addWidget(label_sliderX);

        sliderX1 = new QSlider(groupBox_changeCoverBox);
        sliderX1->setObjectName(QStringLiteral("sliderX1"));
        sliderX1->setMinimumSize(QSize(50, 20));
        sliderX1->setMaximumSize(QSize(50, 20));
        sliderX1->setMinimum(0);
        sliderX1->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(sliderX1);

        sliderX2 = new QSlider(groupBox_changeCoverBox);
        sliderX2->setObjectName(QStringLiteral("sliderX2"));
        sliderX2->setMinimumSize(QSize(50, 20));
        sliderX2->setMaximumSize(QSize(50, 20));
        sliderX2->setMinimum(0);
        sliderX2->setValue(99);
        sliderX2->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(sliderX2);


        verticalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_sliderY = new QLabel(groupBox_changeCoverBox);
        label_sliderY->setObjectName(QStringLiteral("label_sliderY"));
        label_sliderY->setMinimumSize(QSize(21, 16));
        label_sliderY->setMaximumSize(QSize(21, 16));

        horizontalLayout_2->addWidget(label_sliderY);

        sliderY1 = new QSlider(groupBox_changeCoverBox);
        sliderY1->setObjectName(QStringLiteral("sliderY1"));
        sliderY1->setMinimumSize(QSize(50, 20));
        sliderY1->setMaximumSize(QSize(50, 20));
        sliderY1->setMinimum(0);
        sliderY1->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(sliderY1);

        sliderY2 = new QSlider(groupBox_changeCoverBox);
        sliderY2->setObjectName(QStringLiteral("sliderY2"));
        sliderY2->setMinimumSize(QSize(50, 20));
        sliderY2->setMaximumSize(QSize(50, 20));
        sliderY2->setMinimum(0);
        sliderY2->setValue(99);
        sliderY2->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(sliderY2);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_silderZ = new QLabel(groupBox_changeCoverBox);
        label_silderZ->setObjectName(QStringLiteral("label_silderZ"));
        label_silderZ->setMinimumSize(QSize(21, 16));
        label_silderZ->setMaximumSize(QSize(21, 16));

        horizontalLayout_3->addWidget(label_silderZ);

        sliderZ1 = new QSlider(groupBox_changeCoverBox);
        sliderZ1->setObjectName(QStringLiteral("sliderZ1"));
        sliderZ1->setMinimumSize(QSize(50, 20));
        sliderZ1->setMaximumSize(QSize(50, 20));
        sliderZ1->setMinimum(0);
        sliderZ1->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(sliderZ1);

        sliderZ2 = new QSlider(groupBox_changeCoverBox);
        sliderZ2->setObjectName(QStringLiteral("sliderZ2"));
        sliderZ2->setMinimumSize(QSize(50, 20));
        sliderZ2->setMaximumSize(QSize(50, 20));
        sliderZ2->setMinimum(0);
        sliderZ2->setValue(99);
        sliderZ2->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(sliderZ2);


        verticalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(groupBox_changeCoverBox);


        horizontalLayout_9->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        widget_AirPhoto = new AirPhoto(SceneControllerWindow);
        widget_AirPhoto->setObjectName(QStringLiteral("widget_AirPhoto"));
        widget_AirPhoto->setMinimumSize(QSize(400, 300));
        widget_AirPhoto->setMaximumSize(QSize(10000, 300));

        horizontalLayout_6->addWidget(widget_AirPhoto);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        pushButton_roamClear = new QPushButton(SceneControllerWindow);
        pushButton_roamClear->setObjectName(QStringLiteral("pushButton_roamClear"));
        pushButton_roamClear->setMinimumSize(QSize(75, 23));
        pushButton_roamClear->setMaximumSize(QSize(75, 23));

        horizontalLayout_7->addWidget(pushButton_roamClear);

        pushButton_roamStart = new QPushButton(SceneControllerWindow);
        pushButton_roamStart->setObjectName(QStringLiteral("pushButton_roamStart"));
        pushButton_roamStart->setMinimumSize(QSize(50, 23));
        pushButton_roamStart->setMaximumSize(QSize(50, 23));

        horizontalLayout_7->addWidget(pushButton_roamStart);

        label_currentPosition = new QLabel(SceneControllerWindow);
        label_currentPosition->setObjectName(QStringLiteral("label_currentPosition"));
        label_currentPosition->setMinimumSize(QSize(50, 20));
        label_currentPosition->setMaximumSize(QSize(50, 20));

        horizontalLayout_7->addWidget(label_currentPosition);

        label_currentPositionValue = new QLabel(SceneControllerWindow);
        label_currentPositionValue->setObjectName(QStringLiteral("label_currentPositionValue"));
        label_currentPositionValue->setMinimumSize(QSize(150, 20));
        label_currentPositionValue->setMaximumSize(QSize(150, 20));

        horizontalLayout_7->addWidget(label_currentPositionValue);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_7);


        horizontalLayout_9->addLayout(verticalLayout);


        retranslateUi(SceneControllerWindow);

        QMetaObject::connectSlotsByName(SceneControllerWindow);
    } // setupUi

    void retranslateUi(QWidget *SceneControllerWindow)
    {
        SceneControllerWindow->setWindowTitle(QApplication::translate("SceneControllerWindow", "Form", Q_NULLPTR));
        groupBox_measureDistance->setTitle(QApplication::translate("SceneControllerWindow", "\350\267\235\347\246\273\346\265\213\351\207\217", Q_NULLPTR));
        label_pointA->setText(QApplication::translate("SceneControllerWindow", "\347\202\271A", Q_NULLPTR));
        label_pointAValue->setText(QApplication::translate("SceneControllerWindow", "( 0.00, 0.00, 0.00)", Q_NULLPTR));
        label_pointB->setText(QApplication::translate("SceneControllerWindow", "\347\202\271B", Q_NULLPTR));
        label_pointBValue->setText(QApplication::translate("SceneControllerWindow", "( 0.00, 0.00, 0.00)", Q_NULLPTR));
        label_distanceAB->setText(QApplication::translate("SceneControllerWindow", "\350\267\235\347\246\273(\347\261\263)", Q_NULLPTR));
        label_distanceABValue->setText(QApplication::translate("SceneControllerWindow", "\351\225\277\345\272\246", Q_NULLPTR));
        groupBox_showButtons->setTitle(QApplication::translate("SceneControllerWindow", "\346\230\276\347\244\272\345\206\205\345\256\271", Q_NULLPTR));
        pushButton_showModel->setText(QApplication::translate("SceneControllerWindow", "\344\270\211\347\273\264\346\250\241\345\236\213", Q_NULLPTR));
        pushButton_showCoverBox->setText(QApplication::translate("SceneControllerWindow", "\345\214\205\345\233\264\347\233\222", Q_NULLPTR));
        pushButton_showOctreeBoxs->setText(QApplication::translate("SceneControllerWindow", "\345\205\253\345\217\211\346\240\221\345\235\227", Q_NULLPTR));
        pushButton_showRoamPath->setText(QApplication::translate("SceneControllerWindow", "\346\274\253\346\270\270\350\275\250\350\277\271", Q_NULLPTR));
        pushButton_showCoordinate->setText(QApplication::translate("SceneControllerWindow", "\345\235\220\346\240\207\350\275\264", Q_NULLPTR));
        pushButton_Empty->setText(QString());
        groupBox_changeCoverBox->setTitle(QApplication::translate("SceneControllerWindow", "\345\214\205\345\233\264\347\233\222\345\260\272\345\257\270", Q_NULLPTR));
        label_sliderX->setText(QApplication::translate("SceneControllerWindow", "x\350\275\264", Q_NULLPTR));
        label_sliderY->setText(QApplication::translate("SceneControllerWindow", "y\350\275\264", Q_NULLPTR));
        label_silderZ->setText(QApplication::translate("SceneControllerWindow", "z\350\275\264", Q_NULLPTR));
        widget_AirPhoto->setProperty("text", QVariant(QString()));
        pushButton_roamClear->setText(QApplication::translate("SceneControllerWindow", "\351\207\215\347\275\256\346\274\253\346\270\270\347\202\271", Q_NULLPTR));
        pushButton_roamStart->setText(QApplication::translate("SceneControllerWindow", "\346\274\253\346\270\270", Q_NULLPTR));
        label_currentPosition->setText(QApplication::translate("SceneControllerWindow", "\345\275\223\345\211\215\345\235\220\346\240\207", Q_NULLPTR));
        label_currentPositionValue->setText(QApplication::translate("SceneControllerWindow", "( 0.00, 0.00, 0.00)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SceneControllerWindow: public Ui_SceneControllerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCENECONTROLLERWINDOW_H
