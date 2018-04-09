#include "hotspotloader.h"
#include <QDebug>

HotSpotLoader::HotSpotLoader(char* pts_path)
{
    hotSpots_cnt = 0;
    has_file = false;
    int len = strlen(pts_path);
    path = new char[len+1];
    strcpy(path, pts_path);
    path[len-3] = 't';
    path[len-2] = 'x';
    path[len-1] = 't';

    this->load();
}

// if has elements to delete, return true and delete it
bool HotSpotLoader::check()
{
    int new_cnt = 0;
    for(int i=0;i<hotSpots.size();++i) {
        if (hotSpots[i]->toDelete()) {
            hotSpots.erase(hotSpots.begin()+i);
        }
        else {
            new_cnt++;
        }
    }
    hotSpots_cnt = new_cnt;
}

bool HotSpotLoader::load(char *_path)
{
    qDebug() << "HOTSPOTLOADER::LOAD start";
    FILE *f = fopen(_path, "r");
    if (!f) {
        f= fopen(this->path, "r");
    }
    else {
        this->path = new char[strlen(_path)+1];
        strcpy(this->path, _path);
    }
    if (f) {
        has_file = true;

        int num;
        fscanf(f, "%d\n", &num);
        float x,y,z;
        char photo_path[FPATHLEN];
        char description[DESCRIPTIONLEN];
        HotSpot *tmp;
        for (int i=0;i<num;++i) {
            fscanf(f, "(%f, %f, %f)\n", &x, &y, &z);
            fscanf(f, "%s\n", photo_path);
            fscanf(f, "%s\n", description);
            tmp = new HotSpot("热点", glm::vec3(x, y, z));
            tmp->setPhoto(photo_path);
            tmp->setDescription(description);
            hotSpots.push_back(tmp);
            hotSpots_cnt++;
        }
        fclose(f);
        qDebug() << "HOTSPOTLOADER::LOAD end";
        return 0;
    }
    qDebug() << "HOTSPOTLOADER::LOAD end";
    return -1;
}

bool HotSpotLoader::save()
{
    qDebug() << "HOTSPOTLOADER::SAVE start";
    FILE *f = fopen(this->path, "w");
    if (f) {
        fprintf(f, "%d\n", hotSpots_cnt);
        HotSpot *hotSpot;
        for (int i=0;i<hotSpots_cnt;++i) {
            hotSpot = hotSpots[i];
            fprintf(f, "(%.2f, %.2f, %.2f)\n", hotSpot->position.x, hotSpot->position.y, hotSpot->position.z);
            fprintf(f, "%s\n", hotSpot->fpath);
            fprintf(f, "%s\n", hotSpot->description);
        }
        fclose(f);
        qDebug() << "HOTSPOTLOADER::SAVE end";
        return 0;
    }
    qDebug() << "HOTSPOTLOADER::SAVE end";
    return -1;
}

int HotSpotLoader::size()
{
    return hotSpots_cnt;
}

int HotSpotLoader::calculateLamda(glm::vec3 near, glm::vec3 far, glm::vec3 pos)
{
    float lamda=-1, tmp_lamda;
    float x,y,z;
    float delta = 0.1f;
    float CoverBoxXmin = pos.x - HOTSPOT_SIZE;
    float CoverBoxXmax = pos.x + HOTSPOT_SIZE;
    float CoverBoxYmin = pos.y - HOTSPOT_SIZE;
    float CoverBoxYmax = pos.y + HOTSPOT_SIZE;
    float CoverBoxZmin = pos.z - HOTSPOT_SIZE;
    float CoverBoxZmax = pos.z + HOTSPOT_SIZE;
    if (CoverBoxXmin-delta<near.x && near.x<CoverBoxXmax+delta && CoverBoxYmin-delta<near.y && near.y<CoverBoxYmax+delta && CoverBoxZmin-delta<near.z && near.z<CoverBoxZmax+delta) {
        return 0.0f;
    }
    if (far.x!=near.x) {
        // face x_min
        tmp_lamda = (CoverBoxXmin-near.x)/(far.x-near.x);
        y = (1-tmp_lamda)*near.y + tmp_lamda*far.y;
        z = (1-tmp_lamda)*near.z + tmp_lamda*far.z;
        if (tmp_lamda>0 && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
        // face x_max
        tmp_lamda = (CoverBoxXmax-near.x)/(far.x-near.x);
        y = (1-tmp_lamda)*near.y + tmp_lamda*far.y;
        z = (1-tmp_lamda)*near.z + tmp_lamda*far.z;
        if (tmp_lamda>0 && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
    }
    if (far.y!=near.y) {
        // face y_min
        tmp_lamda = (CoverBoxYmin-near.y)/(far.y-near.y);
        x = (1-tmp_lamda)*near.x + tmp_lamda*far.x;
        z = (1-tmp_lamda)*near.z + tmp_lamda*far.z;
        if (tmp_lamda>0 && CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
        // face y_max
        tmp_lamda = (CoverBoxYmax-near.y)/(far.y-near.y);
        x = (1-tmp_lamda)*near.x + tmp_lamda*far.x;
        z = (1-tmp_lamda)*near.z + tmp_lamda*far.z;
        if (tmp_lamda>0 && CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
    }
    if (far.z!=near.z) {
        // face z_min
        tmp_lamda = (CoverBoxZmin-near.z)/(far.z-near.z);
        x = (1-tmp_lamda)*near.x + tmp_lamda*far.x;
        y = (1-tmp_lamda)*near.y + tmp_lamda*far.y;
        if (tmp_lamda>0 && CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
        // face z_max
        tmp_lamda = (CoverBoxZmax-near.z)/(far.z-near.z);
        x = (1-tmp_lamda)*near.x + tmp_lamda*far.x;
        y = (1-tmp_lamda)*near.y + tmp_lamda*far.y;
        if (tmp_lamda>0 && CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta) {
            if (lamda<0) {
                lamda = tmp_lamda;
            }
            else {
                return lamda<tmp_lamda ? lamda : tmp_lamda;
            }
        }
    }
    return lamda;
}

int HotSpotLoader::openHotSpot(glm::vec3 near, glm::vec3 far)
{
    glm::vec3 pos;

    for (int i=0;i<hotSpots_cnt;++i) {
        pos = hotSpots[i]->position;
        if (calculateLamda(near, far, pos)>-1) {
            hotSpots[i]->show();
            return i;
        }
    }
    return -1;
}

int HotSpotLoader::addHotSpot(float x, float y, float z, char* photo_path)
{
    HotSpot *tmp;

    tmp = new HotSpot("热点", glm::vec3(x, y, z));
    tmp->setPhoto(photo_path);
    hotSpots.push_back(tmp);
    tmp->show();

    return hotSpots_cnt++;
}
