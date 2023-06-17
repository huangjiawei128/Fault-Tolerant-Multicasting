#ifndef CUBE_NODE_H
#define CUBE_NODE_H

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "Cube.h"

using namespace std;

class Cube;

class Buffer {
public:
    bool link_used; //  链路是否被占用
    int r;  //  可用的flit数

public:
    //  将链路的可用容量减少n个flit
    void bufferMin(int n);

    //  将链路的可用容量增加n个flit
    void bufferPlus(int n);
};


class CubeNode {
private:
    int n;  //  网络维数
    string id;  //  节点id（字符串形式）
    int digit_id;   //  节点id（数字形式）

public:
    vector<Buffer *> buffers;   //  节点不同方向的buffer
    vector<Buffer *> link_buffers;  //  节点不同方向所连接的邻居节点的buffer
    bool fault; //  节点是否故障
    Cube *cube; //  节点所在的网络拓扑

public:
    //  设置节点所在的网络拓扑及id
    void Initialize(Cube *cube, int digit_id);

    //  根据给定大小设置节点的buffer
    void setBuffer(int buffer_size);

    //  设置节点所连接的邻居节点的buffer
    void setLinkBuffer();

    //  释放节点所有buffer中的链路
    void clearBuffer();

    ~CubeNode() {
        for (int i = 0; i < buffers.size(); ++i) {
            delete buffers[i];
        }
    }
};


/**************************************************

NodeInfo Class define the info of a flit in the cube node

**************************************************/
class NodeInfo {
public:
    int cur;    //  当前节点
    Buffer *buffer; //  当前节点的buffer
    vector<int> dsts;   //  目标节点列表
    unordered_set<int> passed;  //  已经过的节点

public:
    NodeInfo() : cur(-1), buffer(NULL) {}

    NodeInfo(int cur, vector<int> dsts, unordered_set<int> before_passed={}) :
        cur(cur), buffer(NULL), dsts(dsts) {
        passed = before_passed;
        passed.insert(cur);
    }

    NodeInfo(int cur, Buffer *buffer, vector<int> dsts, unordered_set<int> before_passed={}) :
        cur(cur), buffer(buffer), dsts(dsts) {
        passed = before_passed;
        passed.insert(cur);
    }
};

#endif
