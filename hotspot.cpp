#include "hotspot.h"
#include <QPainter>
#include <Qdebug>
#include <QFileDialog>
#include <QPixmapCache>
#include <QTextCodec>

HotSpot::HotSpot(char* title, glm::vec3 position, char* fpath, char* description, QWidget *parent) :
    QWidget(parent)
{
    deleted = false;
    ui = new Ui::HotSpot;
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr(title));
    setPositionLabel(ui->label_position, position.x, position.y, position.z);

    this->position = position;
    if (fpath) {
        strcpy(this->fpath, fpath);
        has_photo = photo.load(fpath);
    }
    else {
        has_photo = false;
    }
    if (description) {
        setDescription(description);
        has_description = true;
    }
    else {
        setDescription("");
        has_description = false;
    }
    if (has_photo) {
        qDebug() << QObject::tr(title) << "has_photo is" << has_photo << ":" << this->fpath;
    }
    // to draw only 1 time
    this->update();

    connect(ui->textEdit_Description, SIGNAL(textChanged()),
            this, SLOT(setDescriptionByTextEdit()));
    connect(ui->pushButton_changePhoto, SIGNAL(clicked(bool)),
            this, SLOT(changePhoto()));
    connect(ui->pushButton_deleteHotSpot, SIGNAL(clicked(bool)),
            this, SLOT(setToDelete()));
}

HotSpot::~HotSpot()
{
    this->~QWidget();
}

bool HotSpot::toDelete()
{
    if (deleted) {
        return true;
    }
    else {
        return false;
    }
}

void HotSpot::setPhoto(char* photo_path)
{
    QString path;
    int fplen;

    if (photo_path==NULL) {
        path = QFileDialog::getOpenFileName(
                    this,
                    "打开图片",
                    "./resource",
                    "*.png");
        QByteArray ba = path.toLocal8Bit();
        fplen = strlen(ba.data());
        if (fplen) {
            strcpy(this->fpath, ba.data());
        }
    }
    else {
        path = QString(QLatin1String(photo_path));
        fplen = strlen(photo_path);
        if (fplen) {
            strcpy(this->fpath, photo_path);
        }
    }
    QPixmapCache::clear();
    if (photo.load(path)) {
        has_photo = true;
        this->update();
    }

}

void HotSpot::setDescription(char* description)
{
    strcpy(this->description, description);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    ui->textEdit_Description->setText(QString::fromLocal8Bit(QByteArray(this->description)));
}

void HotSpot::setDescriptionByTextEdit()
{
    QByteArray ba = ui->textEdit_Description->toPlainText().toLocal8Bit();
    strcpy(this->description, ba.data());
}

void HotSpot::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (has_photo) {
        painter.drawPixmap(ui->widget_Photo->pos().x(),ui->widget_Photo->pos().y(),ui->widget_Photo->width(), ui->widget_Photo->height(), this->photo);
    }
    if (has_description) {
        ui->textEdit_Description->setText(QObject::tr(description));
    }
    QWidget::paintEvent(event);
}

void HotSpot::changePhoto()
{
    QString path;
    int fplen;

    path = QFileDialog::getOpenFileName(
                this,
                "打开图片",
                "./resource",
                "*.png");
    QByteArray ba = path.toLocal8Bit();
    fplen = strlen(ba.data());
    if (fplen) {
        strcpy(this->fpath, ba.data());
        QPixmapCache::clear();
        if (photo.load(path)) {
            has_photo = true;
            this->update();
        }
    }
}

void HotSpot::setToDelete()
{
    this->deleted = true;
    this->close();
}

void HotSpot::setPositionLabel(QLabel *label, float x, float y, float z)
{
    QString str;
    str.append("位置 (");
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
