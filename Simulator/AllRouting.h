#ifndef ALL_ROUTING_H
#define ALL_ROUTING_H

#include "Cube.h"
#include "Message.h"

class Message;
class Cube;
class NodeInfo;

class AllRouting {
public:
    int n;
    Cube *cube;

    vector<NodeInfo> next;

    virtual vector<NodeInfo> forward(Message &s) { return {}; };
    // implemented by child
};

#endif
