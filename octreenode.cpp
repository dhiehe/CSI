#include "octreenode.h"
#include <vector>
#include <math.h>

OctreeNode::OctreeNode()
{
    ID = -1;
    outline_min = glm::vec3(0.0f, 0.0f, 0.0f);
    outline_max = glm::vec3(0.0f, 0.0f, 0.0f);
    outline_cen = glm::vec3(0.0f, 0.0f, 0.0f);
    draw_min = glm::vec3(0.0f, 0.0f, 0.0f);
    draw_max = glm::vec3(0.0f, 0.0f, 0.0f);
    point_count = 0;
    chosen_count = 0;
    dis_to_draw = 0.0f;
    branch = NULL;
    tree_layer = -1;
    resolution = -1;
    pre_resolution = -1;
    father_node = NULL;
    child_nodes = NULL;
    is_empty = true;
};

OctreeNode::OctreeNode(int newID)
{
    ID = newID;
    outline_min = glm::vec3(0.0f, 0.0f, 0.0f);
    outline_max = glm::vec3(0.0f, 0.0f, 0.0f);
    outline_cen = glm::vec3(0.0f, 0.0f, 0.0f);
    draw_min = glm::vec3(0.0f, 0.0f, 0.0f);
    draw_max = glm::vec3(0.0f, 0.0f, 0.0f);
    point_count = 0;
    chosen_count = 0;
    dis_to_draw = 0.0f;
    branch = NULL;
    tree_layer = -1;
    resolution = -1;
    pre_resolution = -1;
    father_node = NULL;
    child_nodes = NULL;
    is_empty = true;
};

float OctreeNode::calculateDistance(float x, float y, float z)
{
    float xx,yy,zz;
    xx = (outline_min.x+outline_max.x)/2;
    yy = (outline_min.y+outline_max.y)/2;
    zz = (outline_min.z+outline_max.z)/2;

    return sqrt(
                (xx-x)*(xx-x)+
                (yy-y)*(yy-y)+
                (zz-z)*(zz-z));
}

void OctreeNode::calculateD2D()
{
    dis_to_draw = 5 * sqrt(
                (outline_min.x-outline_max.x)*
                (outline_min.x-outline_max.x)+
                (outline_min.y-outline_max.y)*
                (outline_min.y-outline_max.y)+
                (outline_min.z-outline_max.z)*
                (outline_min.z-outline_max.z));
    outline_cen.x = (outline_min.x + outline_max.x) / 2;
    outline_cen.y = (outline_min.y + outline_max.y) / 2;
    outline_cen.z = (outline_min.z + outline_max.z) / 2;

    float d, d1, d2, dx,dy,dz;
    // d equals the maximum delta of x, y, z
    // then times OCTREENODE_DRAWMULTIPLE
    dx = outline_max.x-outline_min.x;
    dy = outline_max.y-outline_min.y;
    dz = outline_max.z-outline_min.z;
    d = dx>dy ? dx :
                dy;
    d = d>dz ? d :
               dz;
    d1 = d*OCTREENODE_DRAW_MULTIPLE;
    d2 = d*OCTREENODE_LOAD_MULTIPLE;
    draw_min.x = outline_cen.x-d1;
    draw_max.x = outline_cen.x+d1;
    draw_min.y = outline_cen.y-d1;
    draw_max.y = outline_cen.y+d1;
    draw_min.z = outline_cen.z-d1;
    draw_max.z = outline_cen.z+d1;
    load_min.x = outline_cen.x-d2;
    load_max.x = outline_cen.x+d2;
    load_min.y = outline_cen.y-d2;
    load_max.y = outline_cen.y+d2;
    load_min.z = outline_cen.z-d2;
    load_max.z = outline_cen.z+d2;
    /*
    d = outline_max.x-outline_min.x;
    draw_min.x = outline_min.x-d*OCTREENODE_DRAWMULTIPLE;
    draw_max.x = outline_max.x+d*OCTREENODE_DRAWMULTIPLE;
    d = outline_max.y-outline_min.y;
    draw_min.y = outline_min.y-d*OCTREENODE_DRAWMULTIPLE;
    draw_max.y = outline_max.y+d*OCTREENODE_DRAWMULTIPLE;
    d = outline_max.z-outline_min.z;
    draw_min.z = outline_min.z-d*OCTREENODE_DRAWMULTIPLE*2;
    draw_max.z = outline_max.z+d*OCTREENODE_DRAWMULTIPLE*2;
    */
}

int OctreeNode::calculateRC(glm::vec3 p)
{
    int RC = 0;
    if (p.x<=outline_min.x) {
        RC += RC_X01;
    }
    else if (p.x<outline_max.x) {
        RC += RC_X00;
    }
    else {
        RC += RC_X10;
    }
    if (p.y<=outline_min.y) {
        RC += RC_Y01;
    }
    else if (p.y<outline_max.y) {
        RC += RC_Y00;
    }
    else {
        RC += RC_Y10;
    }
    if (p.z<=outline_min.z) {
        RC += RC_Z01;
    }
    else if (p.z<outline_max.z) {
        RC += RC_Z00;
    }
    else {
        RC += RC_Z10;
    }
    return RC;
}

void OctreeNode::addNode(int addPointID, glm::vec3 position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    this->addNode(addPointID, x, y, z);
}

void OctreeNode::addNode(int addPointID, float x, float y, float z)
{
    point_IDs.push_back(addPointID);
    point_count++;
    if (is_empty) {
        outline_min.x = x;
        outline_min.y = y;
        outline_min.z = z;
        outline_max.x = x;
        outline_max.y = y;
        outline_max.z = z;
        is_empty = false;
        return ;
    }
    outline_min.x = x < outline_min.x ? x :
        outline_min.x;
    outline_min.y = y < outline_min.y ? y :
        outline_min.y;
    outline_min.z = z < outline_min.z ? z :
        outline_min.z;
    outline_max.x = x > outline_max.x ? x :
        outline_max.x;
    outline_max.y = y > outline_max.y ? y :
        outline_max.y;
    outline_max.z = z > outline_max.z ? z :
        outline_max.z;
}

void OctreeNode::bindPTS(PointCloud *input_pts)
{
    pts = input_pts;
}

bool OctreeNode::isDraw(glm::vec3 v)
{
    if (v.x>draw_min.x && v.x<draw_max.x &&
        -v.z>draw_min.y && -v.z<draw_max.y &&
        v.y>draw_min.z && v.y<draw_max.z
        ) {
        return true;
    }
    else {
        return false;
    }
}

bool OctreeNode::isLoad(glm::vec3 v)
{
    if (v.x>load_min.x && v.x<load_max.x &&
        -v.z>load_min.y && -v.z<load_max.y &&
        v.y>load_min.z && v.y<load_max.z
        ) {
        return true;
    }
    else {
        return false;
    }
}
