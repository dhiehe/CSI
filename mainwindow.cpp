#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("现场重建Beta"));
    //this->setAttribute(Qt::WA_DeleteOnClose, true);
    qDebug() << "MAINWINDOW::MAINWINDOW current dir is : " << QDir::currentPath();
    this->setWindowIcon(QIcon("resource/CSI.png"));

    initialized = false;
    scene = new Scene(this);
    //scene->setAttribute(Qt::WA_DeleteOnClose, true);
    ptsPath = NULL;
    pts = new PointCloud();
    timer = new QTimer();
    time_count = 0;

    setConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnect()
{
    connect(ui->pushButton_openScene, SIGNAL(clicked(bool)),
            this, SLOT(onShowScene()));
    connect(ui->pushButton_choosePointCloudPath, SIGNAL(clicked(bool)),
            this, SLOT(onOpenPTS()));
    connect(scene, SIGNAL(xClose()),
            this, SLOT(onCloseScene()));
    connect(timer, SIGNAL(timeout()),
            this, SLOT(addTimeCount()));
}

void MainWindow::addTimeCount()
{
    this->time_count++;
}

void MainWindow::onShowScene()
{
    if (initialized) {
        ui->pushButton_openScene->setEnabled(false);
        this->hide();
        this->scene->showAll();
    }
    else {
        ui->label_pointCloudPath->setText(QObject::tr("请先正确指定点云路径！"));
    }
}

void MainWindow::onOpenPTS()
{
    QString file = QFileDialog::getOpenFileName(
                this,
                "打开点云文件",
                "./resource",
                "*.pts");
    QByteArray ba = file.toLocal8Bit();
    int fplen = strlen(ba.data());
    ptsPath = new char[fplen+1];
    strcpy(ptsPath, ba.data());
    ui->label_pointCloudPath->setEnabled(true);
    ui->label_pointCloudPath->setText(QObject::tr(ptsPath));
    ui->label_readPointCloud->setEnabled(true);
    ui->label_readPointCloud->setText(QObject::tr("正在读取点云..."));
    QTimer::singleShot(1*1000, this, SLOT(onReadPTS()));
}

void MainWindow::onReadPTS()
{
    timer->start(1000);
    int flag = pts->readPTSFile(ptsPath);
    if (flag<0) {
        ui->label_readPointCloud->setText(QObject::tr("点云读取失败"));
        return;
    }
    else {
        ui->label_readPointCloud->setText(QObject::tr("点云读取成功"));
    }
    scene->bindPTS(pts);
    ui->label_buildOctree->setEnabled(true);
    ui->label_buildOctree->setText(QObject::tr("正在建树..."));
    QTimer::singleShot(1*1000, this, SLOT(onBuildPTS()));
}

void MainWindow::onBuildPTS()
{
    octree = new OctreeBuilder();
    int flag = octree->build(pts);
    if (flag<0) {
        ui->label_buildOctree->setText(QObject::tr("八叉树建立失败"));
        return;
    }
    else {
        ui->label_buildOctree->setText(QObject::tr("八叉树建立成功"));
    }
    timer->stop();
    //qDebug() << "cost " << time_count << " seconds"; this isn't reliable
    ui->pushButton_choosePointCloudPath->setEnabled(false);
    scene->bindOctree(octree);
    ui->label_initSceneViewer->setEnabled(true);
    ui->label_initSceneViewer->setText(QObject::tr("正在初始化场景..."));
    QTimer::singleShot(1*1000, this, SLOT(onInitSceneViewer()));
}

void MainWindow::onInitSceneViewer()
{
    this->scene->initViewer();
    ui->label_initSceneViewer->setText(QObject::tr("场景初始化完毕"));
    initialized  = true;
    ui->pushButton_openScene->setEnabled(true);
    this->scene->setDefaultAirPhotoImg(ptsPath);
}

void MainWindow::onCloseScene()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->scene->close();
}
