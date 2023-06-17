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
    int n;
    Cube *cube;

public:
    Routing(Cube *cube) {
        this->cube = cube;
        n = cube->getDimensionsNum();
    }

    void takeBuffer(Buffer *buffer);

    bool testBuffer(Buffer *buffer);

    vector<int> getPossibleDirection(int cur, int dst, unordered_set<int>& passed);

    vector<NodeInfo> forward(Message &s);

    vector<NodeInfo> forwardOne(Message &s, NodeInfo cur_info);
};

#endif
