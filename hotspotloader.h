#ifndef HOTSPOTLOADER_H
#define HOTSPOTLOADER_H

#include "hotspot.h"
#include <vector>
#include <glm/glm.hpp>

using namespace std;

class HotSpotLoader
{
public:
    HotSpotLoader(char* pts_path);
    bool check();
    bool load(char* path=NULL);
    bool save();
    int calculateLamda(glm::vec3 near, glm::vec3 far, glm::vec3 pos);

    int size();
    int openHotSpot(glm::vec3 near, glm::vec3 far);
    int addHotSpot(float x, float y, float z, char* photo_path=NULL);
    vector<HotSpot*> hotSpots;
    int hotSpots_cnt;
    bool has_file;
    char* path;
};

#endif // HOTSPOTLOADER_H
