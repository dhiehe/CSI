#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scene.h"
#include "pointcloud.h"
#include "octreebuilder.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    char *ptsPath;

private:
    void setConnect();

    Ui::MainWindow *ui;
    Scene *scene;
    PointCloud *pts;
    OctreeBuilder *octree;
    bool initialized;
    QTimer *timer;
    int time_count;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onShowScene();
    void onOpenPTS();
    void onReadPTS();
    void onBuildPTS();
    void onInitSceneViewer();
    void onCloseScene();
    void addTimeCount();
};

#endif // MAINWINDOW_H
