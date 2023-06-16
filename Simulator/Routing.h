#ifndef ROUTING_H
#define ROUTING_H

#include <unordered_map>
#include "Message.h"
#include "CubeNode.h"
#include "assert.h"

class Cube;

class Buffer;

class Routing {
public:
    int n;
    Cube *cube;
    vector<NodeInfo> next;

public:
    Routing(Cube *cube) {
        this->cube = cube;
        n = cube->getDimensionsNum();
        next = {};
    }

    void takeBuffer(Buffer* buffer);
    bool testBuffer(Buffer* buffer);
    vector<NodeInfo> forward(Message &s);
    vector<NodeInfo> forwardOne(Message &s, NodeInfo cur_info);
};


#endif