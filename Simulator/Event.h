#ifndef   EVENT_H
#define   EVENT_H

#include <math.h>
#include <algorithm>
#include "Routing.h"
#include "Message.h"
#include "Cube.h"
#include "CubeNode.h"


class Event {
public:
    double total_circle;
    int message_completion_num;
    int message_success_num;
    int total_success_delivery;
    AllRouting *route;
    Cube *cube;
    int n;  // n-cube
    vector<int> fault_nodes_digit_ids;
    vector<int> normal_nodes_digit_ids;

    void setFaultNodes(vector<int> fault_nodes_digit_ids);
    Message *genMsg();  //  generate a message
    void forwardMsg(Message &);//forward a message

    Event(AllRouting *);
};


#endif