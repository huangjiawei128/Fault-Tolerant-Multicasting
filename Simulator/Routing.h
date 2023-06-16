#ifndef ROUTING_H
#define ROUTING_H

#include "AllRouting.h"
#include "Message.h"
#include "CubeNode.h"
#include "assert.h"

class Cube;

class Buffer;

class Routing : public AllRouting {

public:
    Routing(Cube *cube) {
        this->cube = cube;
        n = cube->getDimensionsNum();
        next = {};
    }

    vector<NodeInfo> forward(Message &s);
    vector<NodeInfo> forward(Message &s, CubeNode *cur, vector<CubeNode *> dsts);
};


#endif