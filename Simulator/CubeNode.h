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
    int cur;
    Buffer *buffer;
    vector<int> dsts;
    unordered_set<int> passed;

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
