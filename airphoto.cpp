#include "sceneglobal.h"
#include "airphoto.h"
#include "roamroute.h"
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QPixmapCache>

int airPhotoWidth;
int airPhotoHeight;

AirPhoto::AirPhoto(QWidget *parent) :
    QWidget(parent)
{
    isable_Roam = true;
    timer = NULL;
    photoPath = NULL;
    has_photo = false;
    roamRoute = new RoamRoute();
    setUpdateRate(1000);

    index[0] = "0";
    index[1] = "1";
    index[2] = "2";
    index[3] = "3";
    index[4] = "4";
    index[5] = "5";
    index[6] = "6";
    index[7] = "7";
    index[8] = "8";
    index[9] = "9";

    connect(this, SIGNAL(clickWidget(QPoint,bool)),
            this, SLOT(onClick(QPoint,bool)));
    connect(this, SIGNAL(clickRoamPos(QPoint)),
            this, SLOT(onClickRoamPos(QPoint)));
}

AirPhoto::~AirPhoto()
{
    if (timer!=NULL) {
        timer->~QTimer();
    }
    delete []photoPath;
    this->~QWidget();
}

void AirPhoto::setUpdateRate(int msec)
{
    if (timer!=NULL) {
        timer->~QTimer();
    }
    timer = new QTimer();
    timer->start(msec);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(update()));
}

void AirPhoto::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit AirPhoto::clickWidget(event->pos());
    }
    else if (event->button() == Qt::RightButton) {
        if (event->modifiers() && Qt::ControlModifier) {
            emit AirPhoto::clickRoamPos(event->pos());
            //qDebug() << "AIRPHOTO::MOPUSEPRESSEVENT click ctrl and rightbutton";
        }
        else {
            emit AirPhoto::clickWidget(event->pos(), false);
            //qDebug() << "AIRPHOTO::MOUSEPRESSEVENT click widget position (" << event->pos().x() << ", " << event->pos().y() << ")";
        }
    }
    QWidget::mousePressEvent(event);
}

void AirPhoto::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    /* 画俯视图 */
    if (has_photo) {
        painter.drawPixmap(0,0,this->width(), this->height(), this->vertical_photo);
    }
    /* 画边框 */
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    painter.drawRect(QRect(QPoint(1,1), QPoint(this->width()-2,this->height()-2)));
    int x,y;
    /* 画轨迹点 */
    for (int i=0;i<this->roamRoute->size();++i) {
        painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        x = ROAMX(this->roamRoute, i);
        y = ROAMY(this->roamRoute, i);
        painter.drawRect(QRect(QPoint(x-2,y+4), QPoint(x+2,y+2)));
        painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawText(x-2,y-2,index[i]);
    }
    /* 画轨迹线 */
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap));
    for (int i=1;i<this->roamRoute->size();++i) {
        painter.drawLine(QPoint(ROAMX(this->roamRoute, i-1), ROAMY(this->roamRoute, i-1)), QPoint(ROAMX(this->roamRoute, i), ROAMY(this->roamRoute, i)));
    }
    if (this->roamRoute->size()>2) {
        painter.drawLine(QPoint(ROAMX(this->roamRoute, this->roamRoute->size()-1), ROAMY(this->roamRoute, this->roamRoute->size()-1)), QPoint(ROAMX(this->roamRoute, 0), ROAMY(this->roamRoute, 0)));
    }
    /* 画相机标定
     * 相机位置标定公式与三维场景初始化函数sceneviewer.dataInit中坐标变换有关
     */
    x = (int)(this->width() * (cameraPos.x - CoverBoxXmin) / (CoverBoxXmax - CoverBoxXmin));
    y = (int)(this->height() * (cameraPos.z - (-CoverBoxYmax)) / (CoverBoxYmax - CoverBoxYmin));
    //qDebug() << "AIRPHOTO::PAINTEVENT (cameraPos.z - (-CoverBoxYmax)) / (CoverBoxYmax - CoverBoxYmin)" << cameraPos.z << (-CoverBoxYmax) << CoverBoxYmax << CoverBoxYmin;
    //qDebug() << "AIRPHOTO::PAINTEVENT (x, y) " << x << y;
    painter.setPen(QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter.drawEllipse(x-6, y-6, 12, 12);
    QWidget::paintEvent(event);
}

void AirPhoto::setPhoto()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                "打开航拍图片",
                "./resource",
                "*.png");
    QByteArray ba = path.toLocal8Bit();
    int fplen = strlen(ba.data());
    if (fplen) {
        photoPath = new char[fplen+1];
        strcpy(photoPath, ba.data());
        qDebug() << "AIRPHOTO::SETPHOTO photopath sets to " << ba.data() << " -len " << strlen(ba.data());
        QPixmapCache::clear();
        if (vertical_photo.load(path)) {
            has_photo = true;
        }
    }
}

void AirPhoto::setDefaultPhoto(char *fpath)
{
    int fplen = strlen(fpath);
    if (fplen) {
        photoPath = new char[fplen+1];
        strcpy(photoPath, fpath);
        photoPath[fplen-3] = 'p';
        photoPath[fplen-2] = 'n';
        photoPath[fplen-1] = 'g';
        FILE *f = fopen(photoPath, "r");
        if (f) {
            qDebug() << "AIRPHOTO::SETDEFAULTPHOTO photopath sets to " << photoPath << " -len " << strlen(photoPath);
            if (vertical_photo.load(QString(QLatin1String(photoPath)))) {
                has_photo = true;
            }
            fclose(f);
        }
    }
}

void AirPhoto::setCameraPos(QPoint pos)
{
    cameraPos.x = CoverBoxXmin + ((GLfloat)pos.x()) / this->width() * (CoverBoxXmax - CoverBoxXmin);
    cameraPos.z = -CoverBoxYmax + ((GLfloat)pos.y()) / this->height() * (CoverBoxYmax - CoverBoxYmin);
    //qDebug() << "AIRPHOTO::SETCAMERAPOS set cameraPos to (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")";
}

void AirPhoto::onClick(QPoint pos, bool flag)
{
    if (flag) {
        /* set airphoto */
        //qDebug() << "SCENE::ONCLICKLABELAIRPHOTO Air Photo clicked leftbutton";
        this->setPhoto();
    }
    else {
        /* set camera position */
        //qDebug() << "SCENE::ONCLICKLABELAIRPHOTO Air Photo clicked rightbutton";
        this->setCameraPos(pos);
    }
}

void AirPhoto::onClickRoamPos(QPoint pos)
{
    this->roamRoute->addPos((int)pos.x(), (int)pos.y());
}

void AirPhoto::clearRoam()
{
    this->roamRoute->clear();
}

void AirPhoto::startstopRoam()
{
    if (this->roamRoute->status() == ROAM_STATUS_IDLE) {
        this->roamRoute->activate();
    }
    else {
        this->roamRoute->stop();
    }
}
