#include "scene.h"
#include "octreebuilder.h"
#include "sceneglobal.h"
#include <QDebug>
#include <QDir>

class PointCloud;

Scene::Scene(QWidget *mainwindow, QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::SceneControllerWindow;
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("三维场景控制器"));
    this->sceneViewer = new SceneViewer();
    this->move(1200, 300);
    if_close = false;
    this->mainwindow = mainwindow;
    setConnect();
    this->sceneViewer->bindRoamRoute(ui->widget_AirPhoto->roamRoute);
    timer = NULL;
}

Scene::~Scene()
{
    delete sceneViewer;
    delete ui;
}

void Scene::setConnect()
{
    connect(ui->pushButton_showCoverBox, SIGNAL(clicked(bool)),
            this, SLOT(onSetViewerCoverBox()));
    connect(ui->pushButton_showModel, SIGNAL(clicked(bool)),
            this, SLOT(onSetViewerModel()));
    connect(ui->pushButton_showOctreeBoxs, SIGNAL(clicked(bool)),
            this, SLOT(onSetViewerOctreeCoverBox()));
    connect(ui->pushButton_showRoamPath, SIGNAL(clicked(bool)),
            this, SLOT(onSetViewerRoamPath()));
    connect(ui->pushButton_showCoordinate, SIGNAL(clicked(bool)),
            this, SLOT(onSetViewerCoordinate()));
    connect(ui->pushButton_Empty, SIGNAL(clicked(bool)),
            this, SLOT(onShowHotSpotTest()));
    connect(ui->pushButton_roamClear, SIGNAL(clicked(bool)),
            this, SLOT(onClickRoamClearButton()));
    connect(ui->pushButton_roamStart, SIGNAL(clicked(bool)),
            this, SLOT(onClickRoamStartButton()));
    connect(ui->sliderX1, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryX1(int)));
    connect(ui->sliderX2, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryX2(int)));
    connect(ui->sliderY1, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryY1(int)));
    connect(ui->sliderY2, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryY2(int)));
    connect(ui->sliderZ1, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryZ1(int)));
    connect(ui->sliderZ2, SIGNAL(valueChanged(int)),
            this, SLOT(setSceneViewerBoundaryZ2(int)));
}

void Scene::initViewer()
{
    this->sceneViewer->Init();
}

void Scene::setDefaultAirPhotoImg(char *fpath)
{
    ui->widget_AirPhoto->setDefaultPhoto(fpath);
}

void Scene::bindPTS(PointCloud *pts)
{
    sceneViewer->bindPTS(pts);
}
void Scene::bindOctree(OctreeBuilder *octree)
{
    sceneViewer->bindOctree(octree);
    ui->widget_AirPhoto->setFixedWidth(ui->widget_AirPhoto->height()*(CoverBoxXmax-CoverBoxXmin)/(CoverBoxYmax-CoverBoxYmin));
    airPhotoWidth = ui->widget_AirPhoto->width();
    airPhotoHeight = ui->widget_AirPhoto->height();
    //qDebug() << "SCENE::BINDOCTREE set widget_AirPhoto width to " << ui->widget_AirPhoto->width();
}

void Scene::showAll()
{
    this->show();
    setUpdateRate(1000);
    this->sceneViewer->show();
}

void Scene::onSetViewerCoverBox()
{
    this->sceneViewer->setShowCoverBox(true);
}

void Scene::onSetViewerModel()
{
    this->sceneViewer->setShowModel(true);
}

void Scene::onSetViewerOctreeCoverBox()
{
    this->sceneViewer->setShowOctreeCoverBox(true);
}

void Scene::onSetViewerRoamPath()
{
    this->sceneViewer->setShowRoamPath(true);
}

void Scene::onSetViewerCoordinate()
{
    this->sceneViewer->setShowCoordinate(true);
}

void Scene::onClickRoamClearButton()
{
    ui->widget_AirPhoto->clearRoam();
    ui->pushButton_showRoamPath->setEnabled(false);
}

void Scene::onClickRoamStartButton()
{
    ui->widget_AirPhoto->startstopRoam();
    ui->pushButton_showRoamPath->setEnabled(true);
}

void Scene::onShowHotSpotTest()
{
    /*
    HotSpot *tmp;
    tmp = new HotSpot("液压钳", QDir::currentPath().append("/resource/hotspot1.png").toLatin1().data(), "实验证物1\n液压钳");
    tmp->show();
    hotSpots.push_back(tmp);
    tmp = new HotSpot("手锯", QDir::currentPath().append("/resource/hotspot2.png").toLatin1().data(), "实验证物2\n手锯");
    tmp->show();
    hotSpots.push_back(tmp);
    */
}

void Scene::closeEvent(QCloseEvent *event)
{
    sceneViewer->close();
    emit this->xClose();
}

void Scene::setUpdateRate(int msec)
{
    if (timer!=NULL) {
        timer->~QTimer();
    }
    timer = new QTimer();
    timer->start(msec);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(update()));
}

void Scene::update()
{
    setPositionLabel(ui->label_pointAValue, point1);
    setPositionLabel(ui->label_pointBValue, point2);
    setPositionLabel(ui->label_currentPositionValue, cameraPos.x, -cameraPos.z, cameraPos.y);
    float x, y, z, distance;
    x = point1.x-point2.x;
    y = point1.y-point2.y;
    z = point1.z-point2.z;
    distance = sqrt(x*x+y*y+z*z);
    ui->label_distanceABValue->setText(QObject::tr(QString::number(distance).toLocal8Bit().data()));
    QWidget::update();
}

void Scene::setPositionLabel(QLabel *label, glm::vec3 p)
{
    QString str;
    str.append("(");
    if (p.x>=0) {
        str.append(" ");
    }
    str.append(QString::number(p.x,10,2));
    str.append(", ");
    if (p.y>=0) {
        str.append(" ");
    }
    str.append(QString::number(p.y,10,2));
    str.append(", ");
    if (p.z>=0) {
        str.append(" ");
    }
    str.append((QString::number(p.z,10,2)));
    str.append(")");

    label->setText(str);
}

void Scene::setPositionLabel(QLabel *label, float x, float y, float z)
{
    QString str;
    str.append("(");
    if (x>=0) {
        str.append(" ");
    }
    str.append(QString::number(x,10,2));
    str.append(", ");
    if (y>=0) {
        str.append(" ");
    }
    str.append(QString::number(y,10,2));
    str.append(", ");
    if (z>=0) {
        str.append(" ");
    }
    str.append((QString::number(z,10,2)));
    str.append(")");

    label->setText(str);
}

void Scene::setSceneViewerBoundaryX1(int num)
{
    this->sceneViewer->setBoundary('x', -1, 1.0 - (float)num/ui->sliderX1->maximum());
}

void Scene::setSceneViewerBoundaryX2(int num)
{
    this->sceneViewer->setBoundary('x', 1, (float)num/ui->sliderX2->maximum());
}

void Scene::setSceneViewerBoundaryY1(int num)
{
    this->sceneViewer->setBoundary('y', -1, 1.0 - (float)num/ui->sliderY1->maximum());
}

void Scene::setSceneViewerBoundaryY2(int num)
{
    this->sceneViewer->setBoundary('y', 1, (float)num/ui->sliderY2->maximum());
}

void Scene::setSceneViewerBoundaryZ1(int num)
{
    this->sceneViewer->setBoundary('z', -1, 1.0 - (float)num/ui->sliderZ1->maximum());
}

void Scene::setSceneViewerBoundaryZ2(int num)
{
    this->sceneViewer->setBoundary('z', 1, (float)num/ui->sliderZ2->maximum());
}
