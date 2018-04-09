#include "octreenode.h"
#include "octreebuilder.h"
#include <stdio.h>
#include <vector>
#include <qDebug>
using namespace std;

OctreeBuilder::OctreeBuilder()
{
    is_empty = true;
    pts = NULL;
    ptsChosen = NULL;
    root = new OctreeNode(0);
    root->father_node = NULL;
    root->ID = 0;
    root->tree_layer = 0;
    octree_nodes.push_back(root);
    totalChosenCount = 0;
    leaf_count = 0;
    zero_count = 0;
};

// 分割与内部点数阈值有关
bool OctreeBuilder::judgeSplit(OctreeNode* node)
{
    if (node->point_count>THRESHOLD*3) {
        return true;
    }
    else {
        return false;
    }
}

// 注册几何点到八叉树结点
void OctreeBuilder::chooseNodes(OctreeNode *node)
{
    if (node->point_count<=THRESHOLD*3) {
        for (int i=0;i<node->point_count;++i) {
            node->chosenIDs.push_back(node->point_IDs[i]);
            this->ptsChosen[node->point_IDs[i]] = true;
            totalChosenCount++;
        }
        node->chosen_count = node->chosenIDs.size();
    }
    else {
        int precision = node->point_count / THRESHOLD >2 ? node->point_count / THRESHOLD :
                                                           3;
        int chosenCount = node->point_count / precision;
        int i,j;
        int index = 0;
        int minIndex;
        float minD,D;
        float cenX, cenY, cenZ;
        // for every <precision> of points, choose the closest one to the center.
        for (i=0;i<chosenCount;i++) {
            cenX = 0.0f;
            cenY = 0.0f;
            cenZ = 0.0f;
            for (j=0;j<precision;j++) {
                cenX += PTSX(pts, node->point_IDs[index+j]);
                cenY += PTSY(pts, node->point_IDs[index+j]);
                cenZ += PTSZ(pts, node->point_IDs[index+j]);
            }
            cenX /= precision;
            cenY /= precision;
            cenZ /= precision;

            minD =
                (cenX-PTSX(pts, node->point_IDs[index]))*
                (cenX-PTSX(pts, node->point_IDs[index]))+
                (cenY-PTSY(pts, node->point_IDs[index]))*
                (cenY-PTSY(pts, node->point_IDs[index]))+
                (cenZ-PTSZ(pts, node->point_IDs[index]))*
                (cenZ-PTSZ(pts, node->point_IDs[index]));
            minIndex = index;
            for(int k=1;k<precision;k++) {
                D =
                (cenX-PTSX(pts, node->point_IDs[index+k]))*
                (cenX-PTSX(pts, node->point_IDs[index+k]))+
                (cenY-PTSY(pts, node->point_IDs[index+k]))*
                (cenY-PTSY(pts, node->point_IDs[index+k]))+
                (cenZ-PTSZ(pts, node->point_IDs[index+k]))*
                (cenZ-PTSZ(pts, node->point_IDs[index+k]));
                if (D<minD) {
                    minD = D;
                    minIndex = index+k;
                }
            }
            node->chosenIDs.push_back(node->point_IDs[minIndex]);
            this->ptsChosen[node->point_IDs[minIndex]] = true;
            totalChosenCount++;

            index += precision;
        }
        node->chosen_count = node->chosenIDs.size();
    }
}

// 空间上平均分割成8块
void OctreeBuilder::split(OctreeNode* node)
{
    /* calculate layer scale */
    if (node->point_count==0) {
        zero_count++;
    }
    if (node->tree_layer>=layer_scale.size()) {
        layer_scale.push_back(1);
    }
    else {
        layer_scale[node->tree_layer]++;
    }
    /* choose nodes */
    if (!judgeSplit(node))
    {
        leaf_count ++;
        node->bindPTS(root->pts);
        node->child_nodes=NULL;
        chooseNodes(node);
        return;
    }
    node->bindPTS(root->pts);
    node->child_nodes = new OctreeNode[OCTREENODE_CHILDRENCOUNT];
    //printf("OCTREEBUILDER::SPLIT node %d seperates to", node->ID);
    for (int i = 0; i<OCTREENODE_CHILDRENCOUNT; i++) {
        node->child_nodes[i].father_node = node;
        node->child_nodes[i].ID = octree_nodes.size();
        node->child_nodes[i].tree_layer = node->tree_layer + 1;
        octree_nodes.push_back(node->child_nodes + i);
        //printf(" %d", node->child_nodes[i].ID);
    }
    //putchar('\n');
    chooseNodes(node);

    /* deliver nodes */
    int flag;
    int index;
    int size = node->point_IDs.size();
    for (int i=0;i<size;++i) {
        flag = 0;
        index = node->point_IDs[i];
        if (ptsChosen[index]) {
            continue;
        }
        flag += PTSZ(pts, index) < (node->outline_min.z + node->outline_max.z) / 2 ?
            0 : 1;
        flag += PTSY(pts, index) < (node->outline_min.y + node->outline_max.y) / 2 ?
            0 : 10;
        flag += PTSX(pts, index) < (node->outline_min.x + node->outline_max.x) / 2 ?
            0 : 100;
        switch (flag) {
        case 0:
            node->child_nodes[0].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 1:
            node->child_nodes[1].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 10:
            node->child_nodes[2].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 11:
            node->child_nodes[3].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 100:
            node->child_nodes[4].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 101:
            node->child_nodes[5].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 110:
            node->child_nodes[6].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        case 111:
            node->child_nodes[7].addNode(index, PTSX(pts, index), PTSY(pts, index), PTSZ(pts, index));
            break;
        default:
            ;
        }
    }
    // zr_11_22
    for (int i=0;i<OCTREENODE_CHILDRENCOUNT;i++) {
        if (node->child_nodes[i].point_count>0) {
            split_queue.push_back(node->child_nodes+i);
            node->child_nodes[i].calculateD2D();
        }
        else {
            zero_count++;
            leaf_count++;
            node->child_nodes[i].bindPTS(root->pts);
            node->child_nodes[i].child_nodes=NULL;
        }
    }
}

int OctreeBuilder::build(PointCloud *input_pts)
{
    if (!is_empty) {
        qDebug() << "八叉树已存在，放弃建树";
        return -1;
    }
    else {
        pts = input_pts;
        ptsChosen = new bool[pts->point_count];
        for (int i=0;i<pts->point_count;i++) {
            ptsChosen[i] = false;
        }
        is_empty = false;
    }
    qDebug() << "OCTREEBUILDER::BUILD point count is " << pts->point_count << pts->point_count;

    for (unsigned int i = 0; i < pts->point_count; i++) {
        root->addNode(i, PTSX(pts, i), PTSY(pts, i), PTSZ(pts, i));
    }
    root->calculateD2D();

    split_queue.push_back(root);
    int split_count = 0;
    while (!split_queue.empty()) {
        split(*(split_queue.begin()));
        split_queue.erase(split_queue.begin());
        split_count++;
    }
    qDebug() << "OCTREEBUILDER::BUILD Octree building accomplished. split count is " << split_count << ", box count is " << octree_nodes.size();
    int sum=0;
    int size = this->octree_nodes.size();
    for (int i=0;i<size;++i) {
        sum += octree_nodes[i]->chosenIDs.size();
    }
    qDebug() << "chosen count is " << totalChosenCount << ", sum is " << sum;
    qDebug() << "zero count is" << zero_count;
    qDebug() << "layer count is" << layer_scale;
    qDebug() << "leaf count is" << leaf_count;
    return 0;
}

void OctreeBuilder::output2txt(OctreeNode* head, char* fname)
{
    FILE* f;
    f = fopen(fname, "w");
    fprintf(f, "OCTREEBUILDER::OUTPUT2TXT this function needs to rewrite\n");
    fclose(f);

    return ;
}

void OctreeBuilder::findDot_test(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id)
{
    /* 一个简单的找点算法
     * 从俯视图找，确定包含标定位置投影的叶子结点，再分别计算最小到投影点的距离，再取最小值。
     */
    float x0 = near.x;
    float y0 = near.y;
    float x,y;
    int octree_node_count = octree_nodes.size();
    *chosen_box_id = -1;
    *chosen_point_id = -1;
    int node_index = -1;
    OctreeNode *node = octree_nodes[node_index];
    int node_count;
    float D,minD;
    int point_id;
    bool b = true;
    qDebug() << "OCTREEBUILDER::FINDDOT_TEST start";
    while(node_index<octree_node_count-1) {
        node = octree_nodes[++node_index];
        while (node->child_nodes!=NULL) {
            node = octree_nodes[++node_index];
        }
        node_count = node->point_count;
        // 计算投影
        //qDebug() << node->ID << "(" << node->outline_min.x << node->outline_max.x << ")(" << node->outline_min.y << node->outline_max.y << ")";
        if (node_count==0 ||
                x0<node->outline_min.x || x0>node->outline_max.x ||
                y0<node->outline_min.y || y0>node->outline_max.y) {
            continue;
        }
        // 投影包含在包围盒内
        for(int j=0;j<node_count;++j) {
            point_id = node->point_IDs[j];
            x = PTSX(pts, point_id);
            y = PTSY(pts, point_id);
            D = (x-x0)*(x-x0)+(y-y0)*(y-y0);
            if (b || D<minD) {
                minD = D;
                *chosen_box_id = node_index;
                *chosen_point_id = point_id;
                b = false;
            }
        }
    }
}

void OctreeBuilder::fillList(OctreeNode *node)
{
    int RC1, RC2;
    if (node->point_count>0) {
        RC1 = node->calculateRC(this->near);
        RC2 = node->calculateRC(this->far);
        if ((RC1&RC2)!=0) {
            return;
        }
        else if (node->child_nodes==NULL){
            intersection_list.push_back(node);
        }
        else {
            for (int i=0;i<OCTREENODE_CHILDRENCOUNT;++i) {
                fillList(node->child_nodes+i);
            }
        }
    }
}

float OctreeBuilder::calculateLamda(OctreeNode *node)
{
    float lamda=-1, tmp_lamda;
    float x,y,z;
    float delta = 0.1f;
    float CoverBoxXmin = node->outline_min.x;
    float CoverBoxXmax = node->outline_max.x;
    float CoverBoxYmin = node->outline_min.y;
    float CoverBoxYmax = node->outline_max.y;
    float CoverBoxZmin = node->outline_min.z;
    float CoverBoxZmax = node->outline_max.z;
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

//void OctreeBuilder::findDot(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id, float *lamda)
void OctreeBuilder::findDot(glm::vec3 near, glm::vec3 far, int *chosen_box_id, int *chosen_point_id, float *lamda, vector<int> &ids)
{
    /* 由kohen-sutherland 裁线算法改进的求交算法
     * 从root开始递归查找
     */
    qDebug() << "OCTREEBUILDER::FINDDOT start";
    this->near = near;
    this->far = far;
    vector<int> ().swap(ids); // 1227
    vector<OctreeNode*> ().swap(intersection_list);
    fillList(this->root);
    qDebug() << "OCTREEBUILDER::FINDDOT intersection_list size is" << intersection_list.size();
    OctreeNode *node;
    int _box_id = -1;
    int _point_id = -1;
    float _lamda, tmp_lamda;
    _lamda = 1.0f; // lamda is infinity
    /*
    for (vector<OctreeNode*>::const_iterator iter = intersection_list.begin(); iter != intersection_list.end(); iter++) {
        node = *iter;
        tmp_lamda = calculateLamda(node);
        if (tmp_lamda>-1 && tmp_lamda<_lamda) {
            _lamda = tmp_lamda;
            id = node->ID;
        }
    }
    *chosen_box_id = id;
    *lamda = _lamda;
    */
    float tmp_d2, _d2 = -1.0f;
    int box_cnt = 0;
    int point_cnt = 0;
    int box_size = 0;
    float ABx = far.x - near.x;
    float ABy = far.y - near.y;
    float ABz = far.z - near.z;
    float AB2 = ABx*ABx + ABy*ABy + ABz*ABz;
    int P_id;
    float APx, APy, APz;
    float AP_dot_AB;
    for (vector<OctreeNode*>::const_iterator iter = intersection_list.begin(); iter != intersection_list.end(); iter++) {
        node = *iter;
        tmp_lamda = calculateLamda(node);
        if (tmp_lamda>-1) {
            ids.push_back(node->ID); // 1227
            box_cnt++;
            box_size = node->point_count;
            for (int i=0;i<box_size;++i) {
                point_cnt++;
                P_id = node->point_IDs[i];
                APx = PTSX(pts, P_id) - near.x;
                APy = PTSY(pts, P_id) - near.y;
                APz = PTSZ(pts, P_id) - near.z;
                AP_dot_AB = APx*ABx + APy*ABy + APz*ABz;
                tmp_d2 = (APx*APx + APy*APy + APz*APz) - (AP_dot_AB*AP_dot_AB) / AB2;
                if (_point_id==-1 || _d2>tmp_d2) {
                    _point_id = P_id;
                    _d2 = tmp_d2;
                }
            }
        }
    }
    //qDebug() << "OCTREEBUILDER::FINDDOT beam near is (" << near.x << "," << near.y << "," << near.z << ") far is (" << far.x << "," << far.y << "," << far.z << ")";
    qDebug() << "OCTREEBUILDER::FINDDOT beam through" << box_cnt << "leaf boxes, totally calculate distance with" << point_cnt << "points";
    if (chosen_box_id) {
        *chosen_box_id = _box_id;
    }
    if (lamda) {
        *lamda = _lamda;
    }
    if (chosen_point_id) {
        *chosen_point_id =_point_id;
    }
}
