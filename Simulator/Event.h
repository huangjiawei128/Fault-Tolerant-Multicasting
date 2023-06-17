#ifndef   EVENT_H
#define   EVENT_H

#include <math.h>
#include <algorithm>
#include <assert.h>
#include "Routing.h"
#include "Message.h"
#include "Cube.h"
#include "CubeNode.h"


class Event {
public:
    int total_cycle;   //   总周期数
    int message_completion_num; //  完成组播的消息数
    int message_success_num;    //  成功完成组播的消息数
    Routing *route; //  路由
    Cube *cube; //  网络拓扑
    int n;  //  网络维数
    vector<int> fault_nodes_digit_ids;  //  故障节点的id列表（数字形式）
    vector<int> normal_nodes_digit_ids; //  正常节点的id列表（数字形式）

public:
    Event(Routing *route);

    //  设置故障节点
    void setFaultNodes(vector<int> fault_nodes_digit_ids);

    //  设置网络拓扑中的局部信息
    void setMscsForCube();

    //  按照uniform流量模式随机生成合法消息
    Message *genMsg();

    //  以flit为单位转发消息
    void forwardMsg(Message &s);    //  forward a message
};

#endif
