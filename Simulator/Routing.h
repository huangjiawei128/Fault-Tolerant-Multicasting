#ifndef ROUT_ING
#define ROUT_ING

#include "AllRouting.h"
#include "Message.h"
#include "CubeNode.h"

class Cube;

class Buffer;

class Routing : public AllRouting {

public:
    Routing(Cube *cube1) {
        cube = cube1;
        k = cube1->getNode();
        next = new NodeInfo();
    }

    int chkWrplnk(CubeNode *cur, CubeNode *dst);//check how many wrap links need from cur to dst.
    bool checkBuffer(Buffer *buff1, int &chn, Buffer *&record);


    NodeInfo *forward(Message &);

    NodeInfo *forward(CubeNode *cur, CubeNode *dst);//return the nextnode of cur.
    NodeInfo *noWrapLinkrt(CubeNode *cur, CubeNode *dst);//from cur to do not need wraplink.
    NodeInfo *oneWrapLinkrt(CubeNode *cur, CubeNode *dst);//from cur to  need  one wraplink.
    NodeInfo *twoWrapLinkrt(CubeNode *cur, CubeNode *dst);//from cur to  need  two wraplink.


    int prefer(Buffer *buff1, Buffer *buff2, int &chn1, int &chn2, Buffer *&buff);

};


#endif