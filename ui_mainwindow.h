/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_pointCloudLoader;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_choosePointCloudPath;
    QLabel *label_pointCloudPath;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_readPointCloud;
    QLabel *label_buildOctree;
    QLabel *label_initSceneViewer;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_buttons;
    QPushButton *pushButton_openScene;
    QSpacerItem *horizontalSpacer_2;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(486, 185);
        MainWindow->setMinimumSize(QSize(0, 185));
        MainWindow->setMaximumSize(QSize(16777215, 185));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_pointCloudLoader = new QGroupBox(centralWidget);
        groupBox_pointCloudLoader->setObjectName(QStringLiteral("groupBox_pointCloudLoader"));
        verticalLayout = new QVBoxLayout(groupBox_pointCloudLoader);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_choosePointCloudPath = new QPushButton(groupBox_pointCloudLoader);
        pushButton_choosePointCloudPath->setObjectName(QStringLiteral("pushButton_choosePointCloudPath"));
        pushButton_choosePointCloudPath->setMaximumSize(QSize(75, 23));

        horizontalLayout->addWidget(pushButton_choosePointCloudPath);

        label_pointCloudPath = new QLabel(groupBox_pointCloudLoader);
        label_pointCloudPath->setObjectName(QStringLiteral("label_pointCloudPath"));
        label_pointCloudPath->setEnabled(false);

        horizontalLayout->addWidget(label_pointCloudPath);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_readPointCloud = new QLabel(groupBox_pointCloudLoader);
        label_readPointCloud->setObjectName(QStringLiteral("label_readPointCloud"));
        label_readPointCloud->setEnabled(false);
        label_readPointCloud->setMinimumSize(QSize(91, 20));
        label_readPointCloud->setMaximumSize(QSize(100, 20));

        horizontalLayout_3->addWidget(label_readPointCloud);

        label_buildOctree = new QLabel(groupBox_pointCloudLoader);
        label_buildOctree->setObjectName(QStringLiteral("label_buildOctree"));
        label_buildOctree->setEnabled(false);
        label_buildOctree->setMinimumSize(QSize(100, 20));
        label_buildOctree->setMaximumSize(QSize(100, 20));

        horizontalLayout_3->addWidget(label_buildOctree);

        label_initSceneViewer = new QLabel(groupBox_pointCloudLoader);
        label_initSceneViewer->setObjectName(QStringLiteral("label_initSceneViewer"));
        label_initSceneViewer->setEnabled(false);
        label_initSceneViewer->setMinimumSize(QSize(100, 20));
        label_initSceneViewer->setMaximumSize(QSize(100, 20));

        horizontalLayout_3->addWidget(label_initSceneViewer);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox_pointCloudLoader);

        horizontalLayout_buttons = new QHBoxLayout();
        horizontalLayout_buttons->setSpacing(6);
        horizontalLayout_buttons->setObjectName(QStringLiteral("horizontalLayout_buttons"));
        pushButton_openScene = new QPushButton(centralWidget);
        pushButton_openScene->setObjectName(QStringLiteral("pushButton_openScene"));
        pushButton_openScene->setEnabled(false);
        pushButton_openScene->setMaximumSize(QSize(75, 23));

        horizontalLayout_buttons->addWidget(pushButton_openScene);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_buttons->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_buttons);

        MainWindow->setCentralWidget(centralWidget);
        groupBox_pointCloudLoader->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 486, 23));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        label_pointCloudPath->setBuddy(pushButton_choosePointCloudPath);
#endif // QT_NO_SHORTCUT

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox_pointCloudLoader->setTitle(QApplication::translate("MainWindow", "\345\212\240\350\275\275\347\202\271\344\272\221", Q_NULLPTR));
        pushButton_choosePointCloudPath->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\202\271\344\272\221", Q_NULLPTR));
        label_pointCloudPath->setText(QApplication::translate("MainWindow", "\347\202\271\344\272\221\346\226\207\344\273\266\350\267\257\345\276\204", Q_NULLPTR));
        label_readPointCloud->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\347\202\271\344\272\221", Q_NULLPTR));
        label_buildOctree->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\345\205\253\345\217\211\346\240\221", Q_NULLPTR));
        label_initSceneViewer->setText(QApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226\345\234\272\346\231\257", Q_NULLPTR));
        pushButton_openScene->setText(QApplication::translate("MainWindow", "\344\270\211\347\273\264\345\234\272\346\231\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
