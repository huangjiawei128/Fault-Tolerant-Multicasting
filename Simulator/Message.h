#ifndef  MESSAGE_H
#define  MESSAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
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
    int length; //  消息长度
    int src;    //  源节点
    vector<int> dsts;   //  目标节点
    int begin_trans;    //  消息从生成至开始传输需要等待的cycle数
    vector<vector<NodeInfo>> rpath; //  第i个flit的所在节点信息列表由rpath[i]记录
    int count;  //  消息完成组播消耗的cycle数
    int fault_delivery; //  无法收到消息的目标节点数
    bool finish;    //  消息是否完成组播

public:
    Message(int src, vector<int> dsts) {
        length = MESSAGE_LENGTH;
        this->src = src;
        this->dsts = dsts;
        begin_trans = PROCESS_TIME;
        vector<NodeInfo> initial_infos;
        initial_infos.push_back(NodeInfo(src, dsts));
        for (int i = 0; i < MESSAGE_LENGTH; i++) {
            rpath.push_back(initial_infos);
        }
        count = 0;
        fault_delivery = 0;
        finish = false;
    }

    ~Message() {

    }
};

#endif
