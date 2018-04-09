#ifndef OCTREENODE_H
#define OCTREENODE_H

#ifndef OCTREENODE_CHILDRENCOUNT
#define OCTREENODE_CHILDRENCOUNT 8
#endif // OCTREENODE_CHILDRENCOUNT

#ifndef OCTREENODE_DRAW_MULTIPLE
#define OCTREENODE_DRAW_MULTIPLE 15
#endif // OCTREENODE_DRAW_MULTIPLE
#ifndef OCTREENODE_LOAD_MULTIPLE
#define OCTREENODE_LOAD_MULTIPLE 20
#endif // OCTREENODE_LOAD_MULTIPLE

#define RC_X01 32
#define RC_X00 0
#define RC_X10 16
#define RC_Y01 4
#define RC_Y00 0
#define RC_Y10 8
#define RC_Z01 1
#define RC_Z00 0
#define RC_Z10 2

#include "pointcloud.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class OctreeNode
{
public:
    OctreeNode();
    OctreeNode(int newID);
    void addNode(int addPointID, glm::vec3 position);
    void addNode(int addPointID, float x, float y, float z);
    void bindPTS(PointCloud *input_pts);
    float calculateDistance(float x, float y, float z);
    void calculateD2D();
    void calculateViewField();
    int calculateRC(glm::vec3 p);
    bool isDraw(glm::vec3 v);
    bool isLoad(glm::vec3 v);

    PointCloud *pts;
    glm::vec3 outline_min;
    glm::vec3 outline_max;
    glm::vec3 outline_cen;
    glm::vec3 draw_min;
    glm::vec3 draw_max;
    glm::vec3 load_min;
    glm::vec3 load_max;
    float ViewField[6];
    float dis_to_draw;
    int point_count;
    int chosen_count;
    OctreeNode *father_node;
    OctreeNode *child_nodes;
    vector<int> point_IDs;
    vector<int> chosenIDs;
    bool branch;
    int ID;
    int tree_layer;
    int resolution;
    int pre_resolution;
    bool is_empty;
};

#endif // OctreeNode_H
