#ifndef ALL_ROUTING_H
#define ALL_ROUTING_H

#include "Cube.h"
#include "Message.h"

class Message;
class Cube;
class NodeInfo;

class AllRouting {
public:
    NodeInfo *next;
    Cube *cube;
    int n;

    virtual NodeInfo *forward(Message &s) { return NULL; };
    // implemented by child
};

#endif
