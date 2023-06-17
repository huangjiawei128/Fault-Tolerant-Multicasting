#ifndef ROUTING_H
#define ROUTING_H

#include <map>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
#include "Message.h"
#include "CubeNode.h"

class Cube;

class Buffer;

class Routing {
public:
    int n;  //  网络维数
    Cube *cube; //  网路拓扑

public:
    Routing(Cube *cube) {
        this->cube = cube;
        n = cube->getDimensionsNum();
    }

    //  检查buffer是否可用
    void takeBuffer(Buffer *buffer);

    //  占用buffer
    bool testBuffer(Buffer *buffer);

    //  得到当前节点对于目标节点可能的下一跳方向
    vector<int> getPossibleDirection(int cur, int dst, unordered_set<int>& passed);

    //  根据消息中首flit所在的所有节点的NodeInfo，得到所有下一跳节点的NodeInfo
    vector<NodeInfo> forward(Message &s);

    //  根据消息中首flit所在的某一个节点的NodeInfo，得到所有下一跳节点的NodeInfo
    vector<NodeInfo> forwardOne(Message &s, NodeInfo cur_info);
};

#endif
