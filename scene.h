#ifndef SCENE_H
#define SCENE_H

#include "ui_scenecontrollerwindow.h"
#include "sceneviewer.h"
#include "airphoto.h"
#include <QCloseEvent>
#include <QTimer>
#include <vector>
using namespace std;

class PointCloud;
class OctreeBuilder;

class Scene : public QWidget
{
    Q_OBJECT

public:
    Scene(QWidget *mainwindow=0, QWidget *parent=0);
    ~Scene();
    void bindPTS(PointCloud *pts);
    void bindOctree(OctreeBuilder *octree);
    void showAll();
    void setDefaultAirPhotoImg(char *fpath);
    void initViewer();

    bool if_close;

signals:
    void xClose();

public slots:
    void onSetViewerModel();
    void onSetViewerCoverBox();
    void onSetViewerOctreeCoverBox();
    void onSetViewerCoordinate();
    void onSetViewerRoamPath();
    void onClickRoamClearButton();
    void onClickRoamStartButton();
    void onShowHotSpotTest();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void update();
    void setSceneViewerBoundaryX1(int num);
    void setSceneViewerBoundaryX2(int num);
    void setSceneViewerBoundaryY1(int num);
    void setSceneViewerBoundaryY2(int num);
    void setSceneViewerBoundaryZ1(int num);
    void setSceneViewerBoundaryZ2(int num);

private:
    void setConnect();
    void setUpdateRate(int msec);
    void setPositionLabel(QLabel *label, glm::vec3 p);
    void setPositionLabel(QLabel *label, float x, float y, float z);

    Ui::SceneControllerWindow *ui;
    SceneViewer *sceneViewer;
    QWidget *mainwindow;

    QTimer *timer;
};

#endif // SCENE_H
