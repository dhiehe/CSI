#ifndef OCTREEBUILDER_H
#define OCTREEBUILDER_H

#include "pointcloud.h"
#include "octreenode.h"
#include <vector>
using namespace std;

#define THRESHOLD 10000

class OctreeBuilder
{
public:
    OctreeBuilder();
    void chooseNodes(OctreeNode *node);
    int build(PointCloud *input_pts);
    void output2txt(OctreeNode* head, char* fname);
    void findDot_test(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id);
    void fillList(OctreeNode *node);
    float calculateLamda(OctreeNode *node);
    //void findDot(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id, float *lamda);
    void findDot(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id, float *lamda, vector<int> &ids);

    PointCloud *pts;
    bool *ptsChosen;
    OctreeNode *root;
    vector<OctreeNode*> octree_nodes;
    vector<OctreeNode*> split_queue;

    vector<int> layer_scale;
    int leaf_count;
    int zero_count;

private:
    bool judgeSplit(OctreeNode* node);
    void split(OctreeNode* node);
    int totalChosenCount;
    vector<OctreeNode*> intersection_list;

    bool is_empty;
    glm::vec3 near;
    glm::vec3 far;
};

#endif // OCTREEBUILDER_H
