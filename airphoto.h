#ifndef AIRPHOTO_H
#define AIRPHOTO_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

class RoamRoute;

class AirPhoto : public QWidget
{
    Q_OBJECT

public:
    explicit AirPhoto(QWidget *parent=0);
    ~AirPhoto();
    void setUpdateRate(int msec);
    void setPhoto();
    void setDefaultPhoto(char *fpath);
    void setCameraPos(QPoint pos);
    RoamRoute *roamRoute;

signals:
    void clickWidget(QPoint pos, bool flag=true);
    void clickRoamPos(QPoint pos);

public slots:
    void clearRoam();
    void startstopRoam();

private slots:
    void onClick(QPoint pos, bool flag);
    void onClickRoamPos(QPoint pos);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QTimer *timer;
    char *photoPath;
    QPixmap vertical_photo;
    bool has_photo;
    QString index[10];
    bool isable_Roam;
};

#endif // AIRPHOTO_H
