#ifndef HOTSPOT_H
#define HOTSPOT_H

#include "ui_hotspot.h"
#include <QWidget>
#include <glm/glm.hpp>

#define HOTSPOT_SIZE 0.05

#define FPATHLEN 1000
#define DESCRIPTIONLEN 1000

class QPainter;

class HotSpot : public QWidget
{
    Q_OBJECT

public:
    HotSpot(char* title, glm::vec3 position, char* fpath = NULL, char* description = NULL, QWidget *parent=0);
    ~HotSpot();
    void setPhoto(char* photo_path);
    void setDescription(char* description);
    void setPositionLabel(QLabel *label, float x, float y, float z);
    bool toDelete();
    glm::vec3 position;

    char fpath[FPATHLEN];
    char description[DESCRIPTIONLEN];

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::HotSpot *ui;
    bool has_photo;
    bool has_description;
    bool deleted;
    QPixmap photo;

private slots:
    void setDescriptionByTextEdit();
    void changePhoto();
    void setToDelete();
};

#endif // HOTSPOT_H
