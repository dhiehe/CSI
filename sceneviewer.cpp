#include "sceneglobal.h"
#include "sceneviewer.h"
#include "scenemode.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <qDebug>
#include <vector>

// 全局变量
GLFWwindow *window;
int width;
int height;
bool firstSet;
bool viewerShouldCalculateDot;
bool viewerShouldFindHotSpot;
SceneMode scene_mode;
GLint camera[3];
GLfloat yaw;
GLfloat pitch;
GLfloat m_roll;
GLfloat m_pitch;
GLfloat aspect;
GLfloat lastX;
GLfloat lastY;
bool rot_camera;
GLfloat cameraSpeedRate;
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
GLfloat cameraSpeed;
double lastFrame;
double currentFrame;

float CoverBoxXmin;
float CoverBoxXmax;
float CoverBoxYmin;
float CoverBoxYmax;
float CoverBoxZmin;
float CoverBoxZmax;

glm::vec3 beam_near;
glm::vec3 beam_far;

glm::vec3 point1;
glm::vec3 point2;

#define PERSPECTIVE_NEAR 0.1f
#define PERSPECTIVE_FAR 1000.0f
#define PARAMETER_CLEAR_COLOR 0.5f
#define PARAMETER_GLPOINTSIZE 2

/*
 * 相机初始化
 */
void dataInit()
{
    firstSet = true;
    viewerShouldCalculateDot = false;
    viewerShouldFindHotSpot = false;
    scene_mode.set(SCENE_MODE_FREEVIEW);
    camera[0] = 0;
    camera[1] = 0;
    camera[2] = 0;
    yaw = -90.0f;
    pitch = 0.0f;
    m_roll = 0.0f;
    m_pitch = -90.0f;
    aspect = 30.0f;
    rot_camera = true;
    cameraSpeed = true;
    cameraSpeedRate = 1.0f;
    cameraPos = glm::vec3((CoverBoxXmax+CoverBoxXmin)/2.0f, (CoverBoxZmax+CoverBoxZmin)/5.0f, -(CoverBoxYmin));
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    lastFrame = 0.0f;
    currentFrame = 0.0f;

    beam_near = glm::vec3(0.0f, 0.0f, 0.0f);
    beam_far = glm::vec3(0.0f, 0.0f, 0.0f);

    point1 = glm::vec3(0.0f, 0.0f, 0.0f);
    point2 = glm::vec3(0.0f, 0.0f, 0.0f);
}

/*
 * 相机俯视图
 */
void setVerticalView()
{
    firstSet = true;
    viewerShouldCalculateDot = false;
    viewerShouldFindHotSpot = false;
    scene_mode.set(SCENE_MODE_FREEVIEW);
    camera[0] = 0;
    camera[1] = 0;
    camera[2] = 0;
    yaw = -90.0f;
    pitch = -89.9f;
    m_roll = 0.0f;
    m_pitch = -89.9f;
    aspect = 30.0f;
    rot_camera = true;
    cameraSpeed = true;
    cameraSpeedRate = 1.0f;
    cameraPos = glm::vec3((CoverBoxXmax+CoverBoxXmin)/2.0f, (CoverBoxXmax-CoverBoxXmin) * 2, -(CoverBoxYmax+CoverBoxYmin)/2.0f);
    cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    lastFrame = 0.0f;
    currentFrame = 0.0f;

    beam_near = glm::vec3(0.0f, 0.0f, 0.0f);
    beam_far = glm::vec3(0.0f, 0.0f, 0.0f);

    point1 = glm::vec3(0.0f, 0.0f, 0.0f);
    point2 = glm::vec3(0.0f, 0.0f, 0.0f);
}

// 用于GLFW的回调函数
void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/*
 * 构造函数
 */
SceneViewer::SceneViewer()
{
    this->pts = NULL;
    this->octree = NULL;
    this->verticesRoampath = NULL;
    this->hotSpots = NULL;

    show_model = true;
    show_octreeCoverBox = false;
    show_roampath = false;
    show_coverBox = false;
    show_coordinate = false;

    chosen_box_id = -1;
    chosen_point_id = -1;
    calculated_dot_cnt = 0;
}

/*
 * 析构函数
 */
SceneViewer::~SceneViewer()
{
    delete []VBOModel;
    delete []VAOModel;

    delete []vertices;
    delete []verticesRoampath;
    for(int i=0;i<octree->octree_nodes.size();++i)
        delete [](verticesOctreeBox[i]);
    delete []verticesOctreeCoverBox;
    delete []indicesOctreeCoverBox;
    delete []verticesOctreeCoverBoxEdge;
    delete []indicesOctreeCoverBoxEdge;
}

/*
 * 注册点云指针pts
 */
void SceneViewer::bindPTS(PointCloud *pts)
{
    this->pts = pts;
}

/*
 * 注册八叉树指针octree
 */
void SceneViewer::bindOctree(OctreeBuilder *octree)
{
    this->octree = octree;
    this->octree_node_count = octree->octree_nodes.size();
    this->octree_node_size = new int[octree_node_count];
    for(int i=0;i<octree_node_count;++i) {
        octree_node_size[i] = octree->octree_nodes[i]->chosen_count;
    }

    coverBox_min.x = octree->root->outline_min.x;
    coverBox_min.y = octree->root->outline_min.y;
    coverBox_min.z = octree->root->outline_min.z;
    coverBox_max.x = octree->root->outline_max.x;
    coverBox_max.y = octree->root->outline_max.y;
    coverBox_max.z = octree->root->outline_max.z;
    changeCoverBox_min = coverBox_min;
    changeCoverBox_max = coverBox_max;

    CoverBoxXmin = coverBox_min.x;
    CoverBoxXmax = coverBox_max.x;
    CoverBoxYmin = coverBox_min.y;
    CoverBoxYmax = coverBox_max.y;
    CoverBoxZmin = coverBox_min.z;
    CoverBoxZmax = coverBox_max.z;
}

/*
 * 注册漫游指针roamRoute
 */
void SceneViewer::bindRoamRoute(RoamRoute *roamRoute)
{
    this->roamRoute = roamRoute;
}

/*
 * 拷贝单个点属性
 */
void SceneViewer::assignVertice(int i, GLfloat *arr, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
{
    arr[i+0] = x;
    arr[i+1] = y;
    arr[i+2] = z;
    arr[i+3] = r;
    arr[i+4] = g;
    arr[i+5] = b;
}

/*
 * 拷贝一个立方体8个顶点的索引
 */
void SceneViewer::assignIndiceCoverBoxEdge(int i, GLuint j, GLuint *arr)
{
    /* 12条棱
     * 01 12 23 30
     * 45 56 67 74
     * 04 15 26 37
     */
    arr[i]    = j  ;arr[i+1]  = j+1;    arr[i+2]  = j+1;arr[i+3]  = j+2;    arr[i+4]  = j+2;arr[i+5]  = j+3;    arr[i+6]  = j+3;arr[i+7]  = j;
    arr[i+8]  = j+4;arr[i+9]  = j+5;    arr[i+10] = j+5;arr[i+11] = j+6;    arr[i+12] = j+6;arr[i+13] = j+7;    arr[i+14] = j+7;arr[i+15] = j+4;
    arr[i+16] = j  ;arr[i+17] = j+4;    arr[i+18] = j+1;arr[i+19] = j+5;    arr[i+20] = j+2;arr[i+21] = j+6;    arr[i+22] = j+3;arr[i+23] = j+7;
}

/*
 * 拷贝包围盒顶点的索引
 */
void SceneViewer::assignIndiceCoverBox(int i, GLuint j, GLuint *a)
{
    /* 6个面12个三角形
     * 021 203
     * 054 501
     * 165 612
     * 276 723
     * 347 430
     * 467 645
     */
    a[i]    = j  ;a[i+1]  = j+2;a[i+2]  = j+1;    a[i+3]  = j+2;a[i+4]  = j  ;a[i+5]  = j+3;
    a[i+6]  = j  ;a[i+7]  = j+5;a[i+8]  = j+4;    a[i+9]  = j+5;a[i+10] = j  ;a[i+11] = j+1;
    a[i+12] = j+1;a[i+13] = j+6;a[i+14] = j+5;    a[i+15] = j+6;a[i+16] = j+1;a[i+17] = j+2;
    a[i+18] = j+2;a[i+19] = j+7;a[i+20] = j+6;    a[i+21] = j+7;a[i+22] = j+2;a[i+23] = j+3;
    a[i+24] = j+3;a[i+25] = j+4;a[i+26] = j+7;    a[i+27] = j+4;a[i+28] = j+3;a[i+29] = j  ;
    a[i+30] = j+4;a[i+31] = j+6;a[i+32] = j+7;    a[i+33] = j+6;a[i+34] = j+4;a[i+35] = j+5;
}

/*
 * 初始化GLFW参数
 */
void SceneViewer::myGLFWInit()
{
    // 初始化GLFW
    glfwInit();
    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
}

/*
 * 初始化GLEW参数
 */
void SceneViewer::myGLEWInit()
{
    // 配置glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return ;
    }

    /* 坐标轴 */
    assignVertice(0 , verticesAxis, 0.0f           , 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    assignVertice(6 , verticesAxis, PERSPECTIVE_FAR, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    assignVertice(12, verticesAxis, 0.0f, 0.0f           , 0.0f, 0.0f, 1.0f, 0.0f);
    assignVertice(18, verticesAxis, 0.0f, PERSPECTIVE_FAR, 0.0f, 0.0f, 1.0f, 0.0f);
    assignVertice(24, verticesAxis, 0.0f, 0.0f, 0.0f           , 0.0f, 0.0f, 1.0f);
    assignVertice(30, verticesAxis, 0.0f, 0.0f, PERSPECTIVE_FAR, 0.0f, 0.0f, 1.0f);
    for(int i=0;i<6;++i) {
        indicesAxis[i] = i;
    }
    glGenVertexArrays(1, &VAOAxis);
    glGenBuffers(1, &VBOAxis);
    glGenBuffers(1, &EBOAxis);
    glBindVertexArray(VAOAxis);
        glBindBuffer(GL_ARRAY_BUFFER, VBOAxis);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*36, verticesAxis, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOAxis);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indicesAxis, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(PTS_POSITION_STRIDE * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	/* 八叉树 */
    VAOModel = new GLuint[octree_node_count];
    glGenVertexArrays(octree_node_count, VAOModel);
    VBOModel = new GLuint;
    glGenBuffers(1, VBOModel);
    EBOModel = new GLuint[octree_node_count];
    glGenBuffers(octree_node_count, EBOModel);
    indicesModel = new GLuint*[octree_node_count];
    int data_size = sizeof(GLfloat)*pts->point_count*PTS_DATA_STRIDE;
    float *data = pts->data;
    for (int i=0;i<octree_node_count;++i) {
        indicesModel[i] = new GLuint[octree_node_size[i]];
        for (int j=0;j<octree_node_size[i];++j) {
            indicesModel[i][j] = octree->octree_nodes[i]->chosenIDs[j];
        }
        glBindVertexArray(VAOModel[i]);
            glBindBuffer(GL_ARRAY_BUFFER, *VBOModel);
                glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOModel[i]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*octree_node_size[i], indicesModel[i], GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, PTS_DATA_STRIDE * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, PTS_DATA_STRIDE * sizeof(GLfloat), (GLvoid*)(PTS_POSITION_STRIDE * sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // 漫游轨迹 VAORoampath
    glGenVertexArrays(1, &VAORoampath);
    glGenBuffers(1, &VBORoampath);

    // 八叉树盒 VAOOctreeCoverBox
    OctreeCoverBoxSize = octree->leaf_count;
    verticesOctreeCoverBox = new GLfloat[OctreeCoverBoxSize*6*8];
    indicesOctreeCoverBox = new GLuint[OctreeCoverBoxSize*3*6*2];
    int index= 0;
    OctreeNode *node;
    GLfloat color;
    for(unsigned int i=0;i<octree->octree_nodes.size();++i) {
        node = octree->octree_nodes[i];
        if (node->child_nodes==NULL && node->point_count>0) {
            color = 0.8f;
            assignVertice(index*6*8+0, verticesOctreeCoverBox, node->outline_min.x, node->outline_min.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+6, verticesOctreeCoverBox, node->outline_max.x, node->outline_min.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+12, verticesOctreeCoverBox, node->outline_max.x, node->outline_max.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+18, verticesOctreeCoverBox, node->outline_min.x, node->outline_max.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+24, verticesOctreeCoverBox, node->outline_min.x, node->outline_min.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+30, verticesOctreeCoverBox, node->outline_max.x, node->outline_min.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+36, verticesOctreeCoverBox, node->outline_max.x, node->outline_max.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+42, verticesOctreeCoverBox, node->outline_min.x, node->outline_max.y, node->outline_max.z, color, color, color);
            assignIndiceCoverBox(index*3*6*2, index*8, indicesOctreeCoverBox);
            index++;
        }
    }
    glGenBuffers(1, &VBOOctreeCoverBox);
    glGenVertexArrays(1, &VAOOctreeCoverBox);
    glGenBuffers(1, &EBOOctreeCoverBox);
    glBindVertexArray(VAOOctreeCoverBox);
        glBindBuffer(GL_ARRAY_BUFFER, VBOOctreeCoverBox);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*OctreeCoverBoxSize*6*8, verticesOctreeCoverBox, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOOctreeCoverBox);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*OctreeCoverBoxSize*3*6*2, indicesOctreeCoverBox, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    OctreeCoverBoxEdgeSize = OctreeCoverBoxSize;
    verticesOctreeCoverBoxEdge = new GLfloat[OctreeCoverBoxSize*6*8];
    indicesOctreeCoverBoxEdge = new GLuint[OctreeCoverBoxSize*2*12];
    index= 0;
    for(unsigned int i=0;i<octree->octree_nodes.size();++i) {
        node = octree->octree_nodes[i];
        if (node->child_nodes==NULL && node->point_count>0) {
            color = 0.0f;
            assignVertice(index*6*8+0, verticesOctreeCoverBoxEdge, node->outline_min.x, node->outline_min.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+6, verticesOctreeCoverBoxEdge, node->outline_max.x, node->outline_min.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+12, verticesOctreeCoverBoxEdge, node->outline_max.x, node->outline_max.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+18, verticesOctreeCoverBoxEdge, node->outline_min.x, node->outline_max.y, node->outline_min.z, color, color, color);
            assignVertice(index*6*8+24, verticesOctreeCoverBoxEdge, node->outline_min.x, node->outline_min.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+30, verticesOctreeCoverBoxEdge, node->outline_max.x, node->outline_min.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+36, verticesOctreeCoverBoxEdge, node->outline_max.x, node->outline_max.y, node->outline_max.z, color, color, color);
            assignVertice(index*6*8+42, verticesOctreeCoverBoxEdge, node->outline_min.x, node->outline_max.y, node->outline_max.z, color, color, color);
            assignIndiceCoverBoxEdge(index*2*12, index*8, indicesOctreeCoverBoxEdge);
            index++;
        }
    }
    glGenBuffers(1, &VBOOctreeCoverBoxEdge);
    glGenVertexArrays(1, &VAOOctreeCoverBoxEdge);
    glGenBuffers(1, &EBOOctreeCoverBoxEdge);
    glBindVertexArray(VAOOctreeCoverBoxEdge);
        glBindBuffer(GL_ARRAY_BUFFER, VBOOctreeCoverBoxEdge);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*OctreeCoverBoxSize*6*8, verticesOctreeCoverBoxEdge, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOOctreeCoverBoxEdge);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*OctreeCoverBoxSize*2*12, indicesOctreeCoverBoxEdge, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 包围盒 VAOCoverBox
    assignVertice(0, verticesCoverBox, coverBox_min.x, coverBox_min.y, coverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(6, verticesCoverBox, coverBox_max.x, coverBox_min.y, coverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(12, verticesCoverBox, coverBox_max.x, coverBox_max.y, coverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(18, verticesCoverBox, coverBox_min.x, coverBox_max.y, coverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(24, verticesCoverBox, coverBox_min.x, coverBox_min.y, coverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(30, verticesCoverBox, coverBox_max.x, coverBox_min.y, coverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(36, verticesCoverBox, coverBox_max.x, coverBox_max.y, coverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(42, verticesCoverBox, coverBox_min.x, coverBox_max.y, coverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignIndiceCoverBoxEdge(0, 0, indicesCoverBox);

    glGenBuffers(1, &VBOCoverBox);
    glGenVertexArrays(1, &VAOCoverBox);
    glGenBuffers(1, &EBOCoverBox);
    glBindVertexArray(VAOCoverBox);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCoverBox);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoverBox), verticesCoverBox, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCoverBox);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCoverBox), indicesCoverBox, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 热点 VAOHotSpot
    assignVertice(0,  verticesHotSpot, -HOTSPOT_SIZE, -HOTSPOT_SIZE, -HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(6,  verticesHotSpot,  HOTSPOT_SIZE, -HOTSPOT_SIZE, -HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(12, verticesHotSpot,  HOTSPOT_SIZE,  HOTSPOT_SIZE, -HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(18, verticesHotSpot, -HOTSPOT_SIZE,  HOTSPOT_SIZE, -HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(24, verticesHotSpot, -HOTSPOT_SIZE, -HOTSPOT_SIZE,  HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(30, verticesHotSpot,  HOTSPOT_SIZE, -HOTSPOT_SIZE,  HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(36, verticesHotSpot,  HOTSPOT_SIZE,  HOTSPOT_SIZE,  HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignVertice(42, verticesHotSpot, -HOTSPOT_SIZE,  HOTSPOT_SIZE,  HOTSPOT_SIZE, 1.0f, 1.0f, 1.0f);
    assignIndiceCoverBox(0, 0, indicesHotSpot);

    glGenBuffers(1, &VBOHotSpot);
    glGenVertexArrays(1, &VAOHotSpot);
    glGenBuffers(1, &EBOHotSpot);
    glBindVertexArray(VAOHotSpot);
        glBindBuffer(GL_ARRAY_BUFFER, VBOHotSpot);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesHotSpot), verticesHotSpot, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOHotSpot);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesHotSpot), indicesHotSpot, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 逆投影连线 */
    for (int i=0;i<12;++i) {
        verticesUnproj[i] = 0.0f;
    }
    glGenVertexArrays(1, &VAOUnproj);
    glGenBuffers(1, &VBOUnproj);
    glBindVertexArray(VAOUnproj);
        glBindBuffer(GL_ARRAY_BUFFER, VBOUnproj);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, verticesUnproj, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 选中的点 */
    /*
    glGenVertexArrays(1, &VAOChosenPoint);
    glGenBuffers(1, &VBOChosenPoint);
    glBindVertexArray(VAOChosenPoint);
        glBindBuffer(GL_ARRAY_BUFFER, VBOChosenPoint);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6, verticesChosenPoint, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    */

    // 着色器程序
    // 创建程序
    shaderProgram = glCreateProgram();
    // 编译链接
    readShader(&vertexShaderSource,"shader/VertexShader.txt");
    readShader(&fragmentShaderSource,"shader/FragmentShader.txt");
    shader(vertexShaderSource, fragmentShaderSource, &shaderProgram);
    shaderProgram_shine = glCreateProgram();
    readShader(&vertexShaderSource_shine,"shader/VertexShader_shine.txt");
    readShader(&fragmentShaderSource_shine,"shader/FragmentShader.txt");
    shader(vertexShaderSource_shine, fragmentShaderSource_shine, &shaderProgram_shine);
}

/*读取着色器文件
 */
int SceneViewer::readShader(char** strdata, char* fpath)
{
    ifstream in(fpath, ios::in);
    if (in.is_open())
    {
        long flength;
        in.seekg(0, ios::end);
        flength = in.tellg();
        *strdata = new char[flength];
        for (int i = 0; i < flength; i++)
            (*strdata)[i] = '\0';
        in.seekg(ios::beg);
        in.read(*strdata, flength);
        in.close();
        return 0;
    }
    else {
        qDebug() << "SCENEVIEWER::READSHADER cannot open file " << fpath;
        return -1;
    }

}

/*绘制漫游轨迹
 */
void SceneViewer::setVerticesRoampath()
{
    if (verticesRoampath!=NULL) {
        delete []verticesRoampath;
    }
    int size = roamRoute->size();
    verticesRoampath = new GLfloat[size*2*6];
    GLfloat rx,ry,x,y;
    GLfloat posz = CoverBoxZmin + 0.1f * (CoverBoxZmax-CoverBoxZmin);
    GLfloat r,g,b;
    r = 0.0f;
    g = 0.0f;
    b = 1.0f;
    rx = (float)ROAMX(roamRoute, 0) / airPhotoWidth;
    x = CoverBoxXmin * (1-rx) + CoverBoxXmax * rx;
    ry = (float)ROAMY(roamRoute, 0) / airPhotoHeight;
    y = -CoverBoxYmax * (1-ry) + (-CoverBoxYmin) * ry;
    assignVertice(0, verticesRoampath, x, -y, posz, r, g, b);
    rx = (float)ROAMX(roamRoute, 1) / airPhotoWidth;
    x = CoverBoxXmin * (1-rx) + CoverBoxXmax * rx;
    ry = (float)ROAMY(roamRoute, 1) / airPhotoHeight;
    y = -CoverBoxYmax * (1-ry) + (-CoverBoxYmin) * ry;
    assignVertice(6, verticesRoampath, x, -y, posz, r, g, b);
    for (int i=1;i<size-1;++i) {
        for(int j=0;j<6;j++) {
            verticesRoampath[i*12+j] = verticesRoampath[i*12+j-6];
        }
        rx = (float)ROAMX(roamRoute, i+1) / airPhotoWidth;
        x = CoverBoxXmin * (1-rx) + CoverBoxXmax * rx;
        ry = (float)ROAMY(roamRoute, i+1) / airPhotoHeight;
        y = -CoverBoxYmax * (1-ry) + (-CoverBoxYmin) * ry;
        assignVertice(i*12+6, verticesRoampath, x, -y, posz, r, g, b);
    }
    for(int k=0;k<6;++k) {
        verticesRoampath[size*2*6-12+k] = verticesRoampath[size*2*6-18+k];
    }
    for(int l=0;l<6;++l) {
        verticesRoampath[size*2*6-6+l] = verticesRoampath[l];
    }

    glBindVertexArray(VAORoampath);
        glBindBuffer(GL_ARRAY_BUFFER, VBORoampath);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * roamRoute->size() * 2 * 6, verticesRoampath, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SceneViewer::Init()
{
    changeCoverBox_max = coverBox_max;
    changeCoverBox_min = coverBox_min;

    myGLFWInit();
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SceneView", nullptr, nullptr);
    glfwHideWindow(window);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);
    myGLEWInit();
    hotSpots = new HotSpotLoader(pts->ptsPath);
}

void SceneViewer::show()
{
    glfwShowWindow(window);

    // 初始化全局变量
    dataInit();

    // 注册函数至回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mousebutton_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // 视口 Viewport
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bool isDraw;
    OctreeNode *node;
    vector<int> ids;//1227

    GLfloat clr = 1.0f;
    bool is_add = false;
    GLfloat sub = -0.02f;
    GLfloat add = 0.01f;

    // 画圆形点
    glEnable(GL_POINT_SMOOTH);

    // 设置游戏循环 Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // loop1.检查事件
        glfwPollEvents();

        // loop2.渲染指令
        glEnable(GL_DEPTH_TEST);
        glClearColor(PARAMETER_CLEAR_COLOR, PARAMETER_CLEAR_COLOR, PARAMETER_CLEAR_COLOR, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 5. 绘制物体
        glUseProgram(shaderProgram);
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        // 应用观察矩阵
        currentFrame = glfwGetTime();
        cameraSpeed = cameraSpeedRate * 5.0f * (currentFrame - lastFrame);
        lastFrame = currentFrame;
        // 摄像机位置
        glm::mat4 view;
        // 旋转
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
        // 平移
        /* 自由视角状态可以键盘移动相机位置 */
        if (scene_mode.is(SCENE_MODE_HOTSPOT)) {
            /* do nothing */
        }
        else if (scene_mode.is(SCENE_MODE_CHOOSEDOT)) {
            /* do nothing */
        }
        else if (scene_mode.is(SCENE_MODE_FREEVIEW)) {
            if (camera[0] == 1 || camera[0] == -1)
            {
                cameraPos += (float)camera[0] * cameraSpeed * cameraFront;
            }
            if (camera[1] == 1 || camera[1] == -1)
            {
                cameraPos -= (float)camera[1] * glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            }
            if (camera[2] == 1 || camera[2] == -1)
            {
                cameraPos += (float)camera[2] * cameraSpeed * cameraUp;
            }
            if (roamRoute->status()!=ROAM_STATUS_IDLE) {
                scene_mode.set(SCENE_MODE_ROAMRUN);
            }
        }
        /* 漫游视角无法键盘操控相机位置 */
        else {
            if (roamRoute->status() == ROAM_STATUS_IDLE) {
                // 停止漫游
                scene_mode.set(SCENE_MODE_FREEVIEW);
            }
            if (roamRoute->status() == ROAM_STATUS_READY) {
                // 启动漫游
                setVerticesRoampath();
                roamRoute->run();
                scene_mode.set(SCENE_MODE_ROAMRUN);
            }
            else if (roamRoute->status() == ROAM_STATUS_PAUSE) {
                // 暂停漫游，此时相机可以旋转但不能平移
                if (scene_mode.is(SCENE_MODE_ROAMRUN)) {
                    roamRoute->run();
                }
            }
            else if (roamRoute->status() == ROAM_STATUS_RUNNING) {
                if (scene_mode.is(SCENE_MODE_ROAMPAUSE)) {
                    roamRoute->pause();
                }
                // if equals current roam positon
                //     roam.next
                // cameraPos += normalize(translation(roam position) - camerapos) * cameraspeed;
                float rx, ry, x,y;
                rx = (float)ROAMX(roamRoute, roamRoute->current_index) / airPhotoWidth;
                x = CoverBoxXmin * (1-rx) + CoverBoxXmax * rx;
                ry = (float)ROAMY(roamRoute, roamRoute->current_index) / airPhotoHeight;
                y = -CoverBoxYmax * (1-ry) + (-CoverBoxYmin) * ry;
                if (x-cameraPos.x<0.1 && x-cameraPos.x>-0.1 &&
                    y-cameraPos.z<0.1 && y-cameraPos.z>-0.1) {
                    roamRoute->nextPos();
                    //qDebug() << "set new pos " << roamRoute->current_index;
                }
                else {
                    cameraPos += glm::normalize(glm::vec3(x,cameraPos.y, y)-cameraPos) * cameraSpeed;
                    //qDebug() << "heading " << ROAMX(roamRoute, roamRoute->current_index) << ROAMY(roamRoute, roamRoute->current_index) << " : " << x << cameraPos.y << y;
                }
            }
        }

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // 应用投影矩阵
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(aspect), (GLfloat)width / height, PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
        // 平行投影
        //projection = glm::ortho((GLfloat)0.0f, (GLfloat)width/100, (GLfloat)0.0f, (GLfloat)height/100, PERSPECTIVE_NEAR, PERSPECTIVE_FAR);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // 应用模型矩阵
        glm::mat4 model;
        model = glm::rotate(model, glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        if (show_coordinate) {
            glBindVertexArray(VAOAxis);
                glLineWidth(2);
                glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        // unproj test
        if (viewerShouldCalculateDot) {
            viewerShouldCalculateDot = false;
            float _lamda;
            octree->findDot(beam_near, beam_far, &(this->chosen_box_id), &(this->chosen_point_id), &_lamda, ids);
            if (chosen_point_id>-1) {
                float x,y,z;
                x = PTSX(pts, this->chosen_point_id);
                y = PTSY(pts, this->chosen_point_id);
                z = PTSZ(pts, this->chosen_point_id);
                calculated_dot.push_back(glm::vec3(x, y, z));
                calculated_dot_cnt++;
                qDebug() << "find dot is (" << x << "," << y << "," << z <<")";
                point1 = point2;
                point2 = glm::vec3(x, y, z);
                assignVertice(0, verticesChosenPoint, point1.x, point1.y, point1.z, 1.0f, 1.0f, 1.0f);
                assignVertice(6, verticesChosenPoint, point2.x, point2.y, point2.z, 1.0f, 1.0f, 1.0f);
                assignVertice(12, verticesChosenPoint, beam_near.x, beam_near.y, beam_near.z, 1.0f, 0.0f, 0.0f);
                assignVertice(18, verticesChosenPoint, beam_far.x, beam_far.y, beam_far.z, 1.0f, 0.0f, 0.0f);
            }
        }
        if (viewerShouldFindHotSpot) {
            viewerShouldFindHotSpot = false;
            int hs_id = hotSpots->openHotSpot(beam_near, beam_far);
            if (hs_id<0) {
                int point_id;
                int itmp;
                float ftmp;
                octree->findDot(beam_near, beam_far, &(itmp), &(point_id), &ftmp, ids);
                if (point_id>-1) {
                    float x,y,z;
                    x = PTSX(pts, point_id);
                    y = PTSY(pts, point_id);
                    z = PTSZ(pts, point_id);
                    qDebug() << "Generate HotSpot";
                    hs_id = hotSpots->addHotSpot(x, y, z);
                }
            }
            else {
                qDebug() << "Editing HotSpot" << hs_id;
            }
        }
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAOUnproj);
            glBindBuffer(GL_ARRAY_BUFFER, VBOUnproj);
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24, verticesChosenPoint, GL_DYNAMIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(1);
                glLineWidth(5);
                glDrawArrays(GL_LINES, 0, 2);
                //glPointSize(5);
                //glDrawArrays(GL_POINTS, 3, 1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        if (show_roampath) {
            glBindVertexArray(VAORoampath);
                glBindBuffer(GL_ARRAY_BUFFER, VBORoampath);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * roamRoute->size() * 2 * 6, verticesRoampath, GL_DYNAMIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);
                    glLineWidth(2);
                    //qDebug() << cameraPos.x << cameraPos.y << cameraPos.z << " - " << verticesRoampath[0] << verticesRoampath[1] << verticesRoampath[2];
                    glDrawArrays(GL_LINES, 0, roamRoute->size()*2);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        if (show_octreeCoverBox) {
            // 1227
            int index = 0;
            GLfloat color;
            for(unsigned int i=0;i<octree->octree_nodes.size();++i) {
                node = octree->octree_nodes[i];
                if (node->child_nodes==NULL && node->point_count>0) {
                    color = 0.8f;
                    bool b = false;
                    for (int i=0;i<ids.size();++i) {
                        if (node->ID==ids[i]) {
                            color = 0.3f;
                        }
                        if (b) {
                            break;
                        }
                    }
                    assignVertice(index*6*8+0, verticesOctreeCoverBox, node->outline_min.x, node->outline_min.y, node->outline_min.z, color, color, color);
                    assignVertice(index*6*8+6, verticesOctreeCoverBox, node->outline_max.x, node->outline_min.y, node->outline_min.z, color, color, color);
                    assignVertice(index*6*8+12, verticesOctreeCoverBox, node->outline_max.x, node->outline_max.y, node->outline_min.z, color, color, color);
                    assignVertice(index*6*8+18, verticesOctreeCoverBox, node->outline_min.x, node->outline_max.y, node->outline_min.z, color, color, color);
                    assignVertice(index*6*8+24, verticesOctreeCoverBox, node->outline_min.x, node->outline_min.y, node->outline_max.z, color, color, color);
                    assignVertice(index*6*8+30, verticesOctreeCoverBox, node->outline_max.x, node->outline_min.y, node->outline_max.z, color, color, color);
                    assignVertice(index*6*8+36, verticesOctreeCoverBox, node->outline_max.x, node->outline_max.y, node->outline_max.z, color, color, color);
                    assignVertice(index*6*8+42, verticesOctreeCoverBox, node->outline_min.x, node->outline_max.y, node->outline_max.z, color, color, color);
                    assignIndiceCoverBox(index*3*6*2, index*8, indicesOctreeCoverBox);
                    index++;
                }
            }
            // 1227
            glBindVertexArray(VAOOctreeCoverBox);
                glBindBuffer(GL_ARRAY_BUFFER, VBOOctreeCoverBox);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*OctreeCoverBoxSize*6*8, verticesOctreeCoverBox, GL_DYNAMIC_DRAW);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOOctreeCoverBox);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*OctreeCoverBoxSize*3*6*2, indicesOctreeCoverBox, GL_DYNAMIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);
                    glDrawElements(GL_TRIANGLES, OctreeCoverBoxSize*3*6*2, GL_UNSIGNED_INT, 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindVertexArray(VAOOctreeCoverBoxEdge);
                glBindBuffer(GL_ARRAY_BUFFER, VBOOctreeCoverBoxEdge);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*OctreeCoverBoxSize*6*8, verticesOctreeCoverBoxEdge, GL_DYNAMIC_DRAW);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOOctreeCoverBoxEdge);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*OctreeCoverBoxSize*2*12, indicesOctreeCoverBoxEdge, GL_DYNAMIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);
                    glLineWidth(2);
                    glDrawElements(GL_LINES, OctreeCoverBoxEdgeSize*2*12, GL_UNSIGNED_INT, 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        if (show_coverBox) {
            glBindVertexArray(VAOCoverBox);
                glBindBuffer(GL_ARRAY_BUFFER, VBOCoverBox);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoverBox), verticesCoverBox, GL_DYNAMIC_DRAW);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCoverBox);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCoverBox), indicesCoverBox, GL_DYNAMIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);
                    glLineWidth(2);
                    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        if (show_model) {
            glPointSize(PARAMETER_GLPOINTSIZE);
            for(int i=0;i<octree_node_count;++i) {
                isDraw = false;
                node = octree->octree_nodes[i];
                if (octree_node_size[i]==0) {
                    continue;
                }
                if (node->tree_layer==0 || node->isDraw(cameraPos)) {
                    isDraw = true;
                }
                if (!isDraw) {
                    continue;
                }
                glBindVertexArray(VAOModel[i]);
                    glDrawElements(GL_POINTS, octree_node_size[i], GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
        glm::mat4 hotspot_model;
        // 激活着色器
        glUseProgram(shaderProgram_shine);
        // 定位uniform
        GLint modelLoc_shine = glGetUniformLocation(shaderProgram_shine, "model");
        GLint viewLoc_shine = glGetUniformLocation(shaderProgram_shine, "view");
        GLint projectionLoc_shine = glGetUniformLocation(shaderProgram_shine, "projection");
        GLint clrLoc_shine = glGetUniformLocation(shaderProgram_shine, "clr");
        glUniformMatrix4fv(viewLoc_shine, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc_shine, 1, GL_FALSE, glm::value_ptr(projection));
        if (clr<0.1f) {
            is_add = true;
        }
        else if (clr>0.99f){
            is_add = false;
        }
        if (is_add) {
            clr += add;
        }
        else {
            clr += sub;
        }
        glUniform1f(clrLoc_shine, clr);
        hotSpots->check();
        for (int i=0;i<hotSpots->size();++i) {
            hotspot_model = glm::translate(model, hotSpots->hotSpots[i]->position);
            glUniformMatrix4fv(modelLoc_shine, 1, GL_FALSE, glm::value_ptr(hotspot_model));
            glBindVertexArray(VAOHotSpot);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // loop3.交换缓冲
        glfwSwapBuffers(window);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwTerminate();
    firstSet = true;
    // 释放内存
    glDeleteBuffers(1, &EBOCoverBox);
    glDeleteBuffers(1, &VBOCoverBox);
    glDeleteBuffers(1, &VAOCoverBox);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void cameraControl(int key, int action)
{
    //只有key为上下左右、action为按下弹起时响应
    if (action == GLFW_PRESS)
    {
        switch (key) {
        case GLFW_KEY_W:
            camera[0] = 1;
            break;
        case GLFW_KEY_S:
            camera[0] = -1;
            break;
        case GLFW_KEY_A:
            camera[1] = 1;
            break;
        case GLFW_KEY_D:
            camera[1] = -1;
            break;
        case GLFW_KEY_Q:
            camera[2] = 1;
            break;
        case GLFW_KEY_E:
            camera[2] = -1;
            break;
        default:
            ;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_S:
            camera[0] = 0;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_D:
            camera[1] = 0;
            break;
        case GLFW_KEY_Q:
        case GLFW_KEY_E:
            camera[2] = 0;
            break;
        default:
            ;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
    width = w;
    height = h;
    //glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    // 改变平移速度
    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            cameraSpeedRate = 3.0f;
        }
        else if (action == GLFW_RELEASE) {
            cameraSpeedRate = 1.0f;
        }
    }
    // 改变旋转对象（相机or模型）
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if(action == GLFW_PRESS)
        {
            rot_camera = false;
        }
        else if (action ==GLFW_RELEASE)
        {
            rot_camera = true;
        }
    }
    // 暂停漫游（漫游视角可用）
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (scene_mode.is(SCENE_MODE_ROAMRUN)) {
            scene_mode.set(SCENE_MODE_ROAMPAUSE);
        }
        else if (scene_mode.is(SCENE_MODE_ROAMPAUSE)) {
            scene_mode.set(SCENE_MODE_ROAMRUN);
        }
    }
    // 查看俯视图（自由视角可用）
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS && scene_mode.is(SCENE_MODE_FREEVIEW)) {
        setVerticalView();
    }
    // 初始化相机状态（自由视角可用）
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && scene_mode.is(SCENE_MODE_FREEVIEW)) {
        dataInit();
    }
    // 逆投影
    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        if (scene_mode.is(SCENE_MODE_CHOOSEDOT)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            scene_mode.setformer();
        }
        else if (scene_mode.is(SCENE_MODE_HOTSPOT)){
            // do nothing
        }
        else {
            scene_mode.set(SCENE_MODE_CHOOSEDOT);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    // 设置热点
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        if (scene_mode.is(SCENE_MODE_HOTSPOT)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            scene_mode.setformer();
        }
        else if (scene_mode.is(SCENE_MODE_CHOOSEDOT)){
            // do nothing
        }
        else {
            scene_mode.set(SCENE_MODE_HOTSPOT);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    cameraControl(key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstSet)
    {
        lastX = xpos;
        lastY = ypos;
        firstSet = false;
    }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标的范围是从下往上的
    lastX = xpos;
    lastY = ypos;
    GLfloat sensitivity = 0.03f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (scene_mode.is(SCENE_MODE_HOTSPOT)) {
        /* do nothing */
    }
    else if (scene_mode.is(SCENE_MODE_CHOOSEDOT)){
        /* do nothing */
    }
    else if (!rot_camera)
    {
        // disable model rotation
        //m_roll += xoffset;
        //m_pitch += yoffset;

        if (m_pitch > 90.0f)
            m_pitch = 89.9f;
        if (m_pitch < -90.0f)
            m_pitch = -89.9f;
    }
    else
    {
        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 90.0f)
            pitch = 89.9f;
        if (pitch < -90.0f)
            pitch = -89.9f;
    }
}

void mousebutton_callback(GLFWwindow *window, int button, int action, int mode)
{
    if ((scene_mode.is(SCENE_MODE_CHOOSEDOT)||scene_mode.is(SCENE_MODE_HOTSPOT)) && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        GLdouble projMatrix[16];
        GLdouble posX1,posY1,posZ1,posX2,posY2,posZ2;
        GLfloat winX,winY,winZ;
        int mouse_x=lastX;
        int mouse_y=lastY;
        GLint viewport[4];
        GLdouble modelMatrix[16];
        glPushMatrix();
        glm::mat4 mat;
        mat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        mat = glm::rotate(mat, glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));
        mat = glm::rotate(mat, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 mat2;
        mat2 = glm::perspective(glm::radians(aspect), (GLfloat)width / height, PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
        glGetIntegerv(GL_VIEWPORT,viewport);
        glPopMatrix();
        winX=(float)mouse_x;
        winY=(float)viewport[3]-(float)mouse_y-1;
        winZ=1;
        for (int i=0;i<16;++i) {
            modelMatrix[i] = (GLdouble)glm::value_ptr(mat)[i];
            projMatrix[i] = (GLdouble)glm::value_ptr(mat2)[i];
        }
        gluUnProject((GLdouble)winX,(GLdouble)winY,(GLdouble)winZ,modelMatrix,projMatrix,viewport,&posX1,&posY1,&posZ1);
        winZ=0;
        // gluUnProject((GLdouble)winX,(GLdouble)winY,(GLdouble)winZ,modelMatrix,projMatrix,viewport,&posX2,&posY2,&posZ2);
        posX2 = cameraPos.x;
        posY2 = -cameraPos.z;
        posZ2 = cameraPos.y;
        // to calculate two intersections of beam with model's coverBox
        float lamda_near=-1, lamda_far=-1, tmp_lamda;
        float x,y,z;
        float delta = 0.1f;
        if (CoverBoxXmin-delta<posX2 && posX2<CoverBoxXmax+delta && CoverBoxYmin-delta<posY2 && posY2<CoverBoxYmax+delta && CoverBoxZmin-delta<posZ2 && posZ2<CoverBoxZmax+delta) {
            lamda_near = 0;
        }
        if (lamda_far<0 && posX1!=posX2) {
            // face x_min
            tmp_lamda = (CoverBoxXmin-posX2)/(posX1-posX2);
            y = (1-tmp_lamda)*posY2 + tmp_lamda*posY1;
            z = (1-tmp_lamda)*posZ2 + tmp_lamda*posZ1;
            if (CoverBoxYmin-delta<y && y<CoverBoxYmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
            // face x_max
            tmp_lamda = (CoverBoxXmax-posX2)/(posX1-posX2);
            y = (1-tmp_lamda)*posY2 + tmp_lamda*posY1;
            z = (1-tmp_lamda)*posZ2 + tmp_lamda*posZ1;
            if (CoverBoxYmin-delta<y && y<CoverBoxYmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
        }
        if (lamda_far<0 && posY1!=posY2) {
            // face y_min
            tmp_lamda = (CoverBoxYmin-posY2)/(posY1-posY2);
            x = (1-tmp_lamda)*posX2 + tmp_lamda*posX1;
            z = (1-tmp_lamda)*posZ2 + tmp_lamda*posZ1;
            if (CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
            // face y_max
            tmp_lamda = (CoverBoxYmax-posY2)/(posY1-posY2);
            x = (1-tmp_lamda)*posX2 + tmp_lamda*posX1;
            z = (1-tmp_lamda)*posZ2 + tmp_lamda*posZ1;
            if (CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxZmin-delta<z && z<CoverBoxZmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
        }
        if (lamda_far<0 && posZ1!=posZ2) {
            // face z_min
            tmp_lamda = (CoverBoxZmin-posZ2)/(posZ1-posZ2);
            x = (1-tmp_lamda)*posX2 + tmp_lamda*posX1;
            y = (1-tmp_lamda)*posY2 + tmp_lamda*posY1;
            if (CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
            // face z_max
            tmp_lamda = (CoverBoxZmax-posZ2)/(posZ1-posZ2);
            x = (1-tmp_lamda)*posX2 + tmp_lamda*posX1;
            y = (1-tmp_lamda)*posY2 + tmp_lamda*posY1;
            if (CoverBoxXmin-delta<x && x<CoverBoxXmax+delta && CoverBoxYmin-delta<y && y<CoverBoxYmax+delta) {
                if (lamda_near<0) {
                    lamda_near = tmp_lamda;
                }
                else if (lamda_far<0) {
                    lamda_far = tmp_lamda;
                }
            }
        }
        if (lamda_far<0 && lamda_near<0) {
            beam_far = glm::vec3(0.0f, 0.0f, 0.0f);
            beam_near = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        else if (lamda_far<0) {
            lamda_far = lamda_near;
            lamda_near = 0;
        }
        else if (lamda_near<0){
            lamda_far = 1;
            lamda_near = 0;
        }
        else if (lamda_far<lamda_near) {
            tmp_lamda = lamda_far;
            lamda_far = lamda_near;
            lamda_near = tmp_lamda;
        }
        beam_far.x = (1-lamda_far)*posX2 + lamda_far*posX1;
        beam_far.y = (1-lamda_far)*posY2 + lamda_far*posY1;
        beam_far.z = (1-lamda_far)*posZ2 + lamda_far*posZ1;
        beam_near.x = (1-lamda_near)*posX2 + lamda_near*posX1;
        beam_near.y = (1-lamda_near)*posY2 + lamda_near*posY1;
        beam_near.z = (1-lamda_near)*posZ2 + lamda_near*posZ1;
        if (scene_mode.is(SCENE_MODE_CHOOSEDOT)) {
            viewerShouldCalculateDot = true;
        }
        else if (scene_mode.is(SCENE_MODE_HOTSPOT)) {
            viewerShouldFindHotSpot = true;
        }

    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (aspect >= 1.0f && aspect <= 45.0f)
        aspect -= yoffset;
    if (aspect <= 1.0f)
        aspect = 1.0f;
    if (aspect >= 45.0f)
        aspect = 45.0f;
}

int SceneViewer::shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource, GLuint* shaderProgram)
{
    // 编译顶点着色器
    GLuint vertexShader;
    // 创建着色器对象为顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 附加源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译
    glCompileShader(vertexShader);
    // 检测编译是否成功
    GLint success;
    GLchar infoLog[512];
    // 检查
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // 报错信息
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    // 编译片段着色器
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENTRED::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    // 附加顶点着色器
    glAttachShader(*shaderProgram, vertexShader);
    // 附加片段着色器
    glAttachShader(*shaderProgram, fragmentShader);
    // 链接
    glLinkProgram(*shaderProgram);
    // 检查
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    // 报错信息
    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAMRED::LINK_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

void SceneViewer::setShowModel(bool flag)
{
    show_model = !show_model;
}

void SceneViewer::setShowOctreeCoverBox(bool flag)
{
    show_octreeCoverBox = !show_octreeCoverBox;
}

void SceneViewer::setShowRoamPath(bool flag)
{
    show_roampath = !show_roampath;
}

void SceneViewer::setShowCoverBox(bool flag)
{
    show_coverBox = !show_coverBox;
}

void SceneViewer::setShowCoordinate(bool flag)
{
    show_coordinate = !show_coordinate;
}

void SceneViewer::close()
{
    if (hotSpots) {
        hotSpots->save();
    }
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneViewer::setBoundary(char axis, int minmax, float num)
{
    // setValue
    if (axis=='x') {
        if (minmax<0) {
            changeCoverBox_min.x = coverBox_min.x * num;
        } else {
            changeCoverBox_max.x = coverBox_max.x * num;
        }
    } else if (axis=='y') {
        if (minmax<0) {
            changeCoverBox_min.y = coverBox_min.y * num;
        } else {
            changeCoverBox_max.y = coverBox_max.y * num;
        }
    } else if (axis=='z') {
        if (minmax<0) {
            changeCoverBox_min.z = coverBox_min.z * num;
        } else {
            changeCoverBox_max.z = coverBox_max.z * num;
        }
    }
    qDebug() << "X" << changeCoverBox_min.x << "<-->" << changeCoverBox_max.x
             << "Y" << changeCoverBox_min.y << "<-->" << changeCoverBox_max.y
             << "Z" << changeCoverBox_min.z << "<-->" << changeCoverBox_max.z;

    // changeData
    assignVertice(0, verticesCoverBox, changeCoverBox_min.x, changeCoverBox_min.y, changeCoverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(6, verticesCoverBox, changeCoverBox_max.x, changeCoverBox_min.y, changeCoverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(12, verticesCoverBox, changeCoverBox_max.x, changeCoverBox_max.y, changeCoverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(18, verticesCoverBox, changeCoverBox_min.x, changeCoverBox_max.y, changeCoverBox_min.z, 1.0f, 0.0f, 0.0f);
    assignVertice(24, verticesCoverBox, changeCoverBox_min.x, changeCoverBox_min.y, changeCoverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(30, verticesCoverBox, changeCoverBox_max.x, changeCoverBox_min.y, changeCoverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(36, verticesCoverBox, changeCoverBox_max.x, changeCoverBox_max.y, changeCoverBox_max.z, 1.0f, 0.0f, 0.0f);
    assignVertice(42, verticesCoverBox, changeCoverBox_min.x, changeCoverBox_max.y, changeCoverBox_max.z, 1.0f, 0.0f, 0.0f);

    glBindVertexArray(VAOCoverBox);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCoverBox);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoverBox), verticesCoverBox, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCoverBox);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCoverBox), indicesCoverBox, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
