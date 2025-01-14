#ifndef CUBE_H
#define CUBE_H

#include <math.h>
#include <assert.h>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "CubeNode.h"
#include "common.h"

using namespace std;

class CubeNode;

enum NodeState {
    FAULTY, L_ORDINARILY_UNSAFE, L_STRONGLY_UNSAFE, L_SAFE
};

enum SubcubeState {
    FULLY_UNSAFE, SAFE
};

typedef unordered_map<string, NodeState> NodeStateMap;

class Cube {
private:
    int n;  //  网络拓扑维数
    int node_num;   //  节点总数
    int buffer_size;    //  buffer最大容量
    CubeNode *head; //  节点列表
    unordered_map<string, NodeStateMap> msc_info;  //  key：msc对应的符号串；value：msc中各节点的状态

public:
    Cube(int n, int buffer_size);

    CubeNode *operator[](string id);

    CubeNode *operator[](int digit_id);

    //  将指定节点置为故障节点
    void setFault(string id);

    void setFault(int digit_id);

    //  判断指定节点是否为故障节点
    bool isFaulty(string id);

    bool isFaulty(int digit_id);

    int getDimensionsNum() { return n; }

    int getNodeNum() { return node_num; }

    ~Cube() {
        delete[] head;
    }

    //  设置网络拓扑的局部信息，主要是maximal safe subcube(msc)及其中各节点的局部状态
    void setMscs();

    //  与网络拓扑信息相关的一系列方法，方法的具体含义如方法名所示
    pair<SubcubeState, NodeStateMap> getSubcubeStateInfo(string subcube);

    vector<string> getSubcubeChildren(string subcube);

    bool isNodeInSubcube(string node_id, string subcube);

    vector<string> getNodesInSubcube(string subcube);

    vector<string> getNodeNeighsInSubcube(string node_id, string subcube);

    NodeState judgeNodeStateByNeighStates(vector<NodeState> neigh_states);

    int getLocalSafetyValueInMsc(string node_id, string subcube);

    unordered_map<string, NodeStateMap> getSameMsc(string node_id1, string node_id2);

    int getDistanceBetweenNode(string node_id1, string node_id2);

    vector<string> getSrcNodeNeighsInMinPath(string src_id, string dst_id);

    vector<string> getSrcNodeNeighsOutMinPath(string src_id, string dst_id);

    int getFirstDiffDirection(string node_id1, string node_id2);
};

#endif
