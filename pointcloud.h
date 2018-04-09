#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <glm/glm.hpp>
#include <vector>
using namespace std;

#define PTS_DATA_STRIDE 7
#define PTS_POSITION_STRIDE 3
#define PTS_COLOR_STRIDE 3
#define PTS_INTENSITY_STRIDE 1

#define PTSX(pts, index) ((pts)->data[index*PTS_DATA_STRIDE])
#define PTSY(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+1])
#define PTSZ(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+2])
#define PTSR(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+3])
#define PTSG(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+4])
#define PTSB(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+5])
#define PTSI(pts, index) ((pts)->data[index*PTS_DATA_STRIDE+6])

class PointCloud
{
public:
    PointCloud();
    ~PointCloud();
    int readPTSFile(char *fpath);

    int point_count;
    char ptsPath[10000];
    float *data;

private:
    bool is_empty;
};

#endif // POINTCLOUD_H
