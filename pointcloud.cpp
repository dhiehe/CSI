#include "pointcloud.h"
#include <QDebug>

#define COLOR_REGULARED

PointCloud::PointCloud()
{
    is_empty = true;
    point_count = 0;
    ptsPath[0] = '\0';
    data = NULL;
}

PointCloud::~PointCloud()
{
    delete []data;
}

int PointCloud::readPTSFile(char *fpath)
{
    if (!is_empty) {
        qDebug() << "已有pts文件，放弃读取";
        return -1;
    }
    else {
        is_empty = false;
    }

    FILE* file;
    file = fopen(fpath, "r");
    if (!file) {
        // fprintf(stderr, "POINTCLOUD::READPTSFILE can't open PointCloud \"%s\".\n",fpath);
        return -1;
    }
    strcpy(ptsPath, fpath);
    fscanf(file, "%d", &point_count);
    data = new float[point_count*PTS_DATA_STRIDE];

    for(int i=0;i<point_count;++i) {
        fscanf(file, "%f %f %f %f %f %f %f",
               &PTSX(this, i), &PTSY(this, i), &PTSZ(this, i),
               &PTSI(this, i),
               &PTSR(this, i), &PTSG(this, i), &PTSB(this, i));
        #ifndef COLOR_REGULARED
        PTSR(this, i) /= 255.0f;
        PTSG(this, i) /= 255.0f;
        PTSB(this, i) /= 255.0f;
        #endif // COLOR_REGULARED
    }
    fclose(file);
    return 0;
}
