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


/**************************************************

NodeInfo Class define the info of a flit in the cube node

**************************************************/
class NodeInfo {
public:
    string node_id;
    Buffer *buffer;
    vector<string> dsts;

public:
    NodeInfo() : node_id(-1) {}
};


class CubeNode {
private:
    int n;
    string id;
    int digit_id;

public:
    vector<Buffer *> buffers;
    vector<Buffer *> link_buffers;
    vector<string> link_nodes;
    bool fault;
    Cube *cube;

public:
    void Initialize(int digit_id);
    void setBuffer(int buffer_size);
    void setLinkBuffer();
    void setCube(Cube *cube);
    void bufferPlus(Buffer *buff, int n);
    void bufferMin(Buffer *buff, int n);
    void clearBuffer();

    ~CubeNode() {
        for (int i=0; i<buffers.size(); ++i) {
            delete buffers[i];
        }
    }
};

#endif