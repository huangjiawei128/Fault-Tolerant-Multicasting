#ifndef CUBE_NODE_H
#define CUBE_NODE_H

#include "Cube.h"
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
using namespace std;

class Cube;

class Buffer {
public:
    bool link_used; //  当前Buffer的链路是否被占用
    int r;  //  当前Buffer的剩余容量

public:
    void bufferMin(int n);
    void bufferPlus(int n);
};


class CubeNode {
private:
    int n;
    string id;
    int digit_id;

public:
    vector<Buffer *> buffers;
    vector<Buffer *> link_buffers;
    bool fault;
    Cube *cube;

public:
    void Initialize(Cube *cube, int digit_id);
    void setBuffer(int buffer_size);
    void setLinkBuffer();
    void bufferPlus(Buffer *buff, int n);
    void bufferMin(Buffer *buff, int n);
    void clearBuffer();

    ~CubeNode() {
        for (int i=0; i<buffers.size(); ++i) {
            delete buffers[i];
        }
    }
};


/**************************************************

NodeInfo Class define the info of a flit in the cube node

**************************************************/
class NodeInfo {
public:
    int cur;
    Buffer *buffer;
    vector<int> dsts;

public:
    NodeInfo() : cur(-1), buffer(NULL) {}
    NodeInfo(int cur, vector<int> dsts) : cur(cur), buffer(NULL), dsts(dsts) {}
    NodeInfo(int cur, Buffer* buffer, vector<int> dsts) : cur(cur), buffer(buffer), dsts(dsts) {}
};

#endif
