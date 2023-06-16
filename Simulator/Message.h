#ifndef  MESSAGE_H
#define  MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "CubeNode.h"
#include "common.h"

using namespace std;

class Buffer;

class NodeInfo;


/************************
    Message info
************************/
class Message {
public:
    int length;            //   measured by flits
    int src;                    //  the source of the message
    vector<int> dsts;   //  the  destinations of the message
    int begin_trans;  //    when a message is generated ,it needs some time until transmitting, begintrans record this
    vector<NodeInfo>* rpath; //    the ith flit now at routpath[i][j].cur and take routpath[i][j].buffer
    int count;  //  the total time a message  consumed
    int fault_delivery;    //  the dsts num delivered faultily
    bool finish;    //  delivery finished?

public:
    Message() {
        src = -1;
        dsts = {};
    }

    Message(int src, vector<int> dsts) {
        length = MESSAGE_LENGTH;
        this->src = src;
        this->dsts = dsts;
        begin_trans = PROCESS_TIME;
        rpath = new vector<NodeInfo>[MESSAGE_LENGTH];
        assert(rpath);
        vector<NodeInfo> initial_infos;
        initial_infos.push_back(NodeInfo(src, dsts));
        for (int i = 0; i < MESSAGE_LENGTH; i++) {
            rpath[i] = initial_infos;
        }
        count = 0;
        fault_delivery = 0;
        finish = false;
    }

    ~Message() {
        delete[] rpath;
    }

    void setLength(int n) {
        length = n;
    }
};

#endif
