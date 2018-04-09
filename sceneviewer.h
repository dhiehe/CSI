#ifndef SCENEVIEWER_H
#define SCENEVIEWER_H

#include "pointcloud.h"
#include "octreebuilder.h"
#include "hotspotloader.h"
#include "roamroute.h"
#include <GL/glew1/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;

class SceneViewer
{
public:
    SceneViewer();
    ~SceneViewer();
    void bindPTS(PointCloud *pts);
    void bindOctree(OctreeBuilder *octree);
    void bindRoamRoute(RoamRoute *roamRoute);
    void myGLFWInit();
    void myGLEWInit();
    void Init();
    void show();
    void close();
    int readShader(char** strdata, char* fpath);

    void cameraControl(int key, int action);
    int shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource, GLuint* shaderProgram);
    void setShowModel(bool);
    void setShowOctreeCoverBox(bool);
    void setShowRoamPath(bool);
    void setShowCoverBox(bool);
    void setShowCoordinate(bool);
    void setBoundary(char axis, int minmax, float num);

    int chosen_box_id;
    int chosen_point_id;

private:
    void assignVertice(int i, GLfloat *arr, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    void assignIndiceCoverBoxEdge(int i,  GLuint j, GLuint *arr);
    void assignIndiceCoverBox(int i,  GLuint j, GLuint *a);
    void setVerticesRoampath();

    /* 顶点着色器 */
    GLchar* vertexShaderSource;
    GLchar* vertexShaderSource_shine;
    /* 片段着色器 */
    GLchar* fragmentShaderSource;
    GLchar* fragmentShaderSource_shine;

    PointCloud *pts;
    OctreeBuilder *octree;
    RoamRoute *roamRoute;
    HotSpotLoader *hotSpots;
    int octree_node_count;
    int *octree_node_size;
    /* 坐标轴 */
    GLuint EBOAxis;
    GLuint VBOAxis;
    GLuint VAOAxis;
    GLfloat verticesAxis[36];
    GLuint indicesAxis[6];
    /* 原始模型 */
    GLuint VBO;
    GLuint VAO;
    GLfloat *vertices;
    /* 八叉树模型 */
    GLuint *VBOModel;
    GLuint *VAOModel;
    GLuint *EBOModel;
    GLfloat **verticesOctreeBox;
    GLuint **indicesModel;
    /* 漫游轨迹 */
    GLuint VBORoampath;
    GLuint VAORoampath;
    GLfloat *verticesRoampath;
    /* 模型包围盒 */
    GLuint VBOCoverBox;
    GLuint VAOCoverBox;
    GLuint EBOCoverBox;
    GLfloat verticesCoverBox[48];
    GLuint indicesCoverBox[24];
    /* 热点 */
    GLuint VBOHotSpot;
    GLuint VAOHotSpot;
    GLuint EBOHotSpot;
    GLfloat verticesHotSpot[48];
    GLuint  indicesHotSpot[36];
    /* 八叉树盒 */
    GLuint VBOOctreeCoverBox;
    GLuint VAOOctreeCoverBox;
    GLuint EBOOctreeCoverBox;
    int OctreeCoverBoxSize;
    GLfloat *verticesOctreeCoverBox;
    GLuint *indicesOctreeCoverBox;
    /* 八叉树盒描边 */
    GLuint VBOOctreeCoverBoxEdge;
    GLuint VAOOctreeCoverBoxEdge;
    GLuint EBOOctreeCoverBoxEdge;
    int OctreeCoverBoxEdgeSize;
    GLfloat *verticesOctreeCoverBoxEdge;
    GLuint *indicesOctreeCoverBoxEdge;
    /* 逆投影连线 */
    GLuint VBOUnproj;
    GLuint VAOUnproj;
    GLfloat verticesUnproj[12];
    /* 选中的点 */
    GLuint VBOChosenPoint;
    GLuint VAOChosenPoint;
    GLfloat verticesChosenPoint[24];

    GLuint shaderProgram;
    GLuint shaderProgram_shine;

    bool show_model;
    bool show_octreeCoverBox;
    bool show_roampath;
    bool show_coverBox;
    bool show_coordinate;

    glm::vec3 coverBox_min;
    glm::vec3 coverBox_max;
    glm::vec3 changeCoverBox_min;
    glm::vec3 changeCoverBox_max;

    vector<glm::vec3> calculated_dot;
    int calculated_dot_cnt;
};

#endif // SCENEVIEWER_H
