#include"Event.h"

Event::Event(AllRouting *route) {
    total_circle = 0;
    message_completion_num = 0;
    message_success_num = 0;
    total_success_delivery = 0;
    route = route;
    cube = route->cube;
    n = route->n;
}

void Event::setFaultNodes(vector<int> fault_nodes_digit_ids) {
    this->fault_nodes_digit_ids = fault_nodes_digit_ids;
    for (auto it=fault_nodes_digit_ids.begin(); it!=fault_nodes_digit_ids.end(); ++it) {
        cube->setFault(*it);
    }
    for (int i=0; i<cube->getNodesNum(); ++i) {
        if (find(fault_nodes_digit_ids.begin(), fault_nodes_digit_ids.end(), i) ==
            fault_nodes_digit_ids.end()) {
            this->normal_nodes_digit_ids.push_back(i);
        }
    }
}

Message *Event::genMsg() {  // generate a message
    Cube *cube = NULL;
    if (route != NULL)
        cube = route->cube;
    //  采用uniform流量模式
    vector<int> dsts = random_select(normal_nodes_digit_ids, DST_NODES_NUM + 1);
    int src = dsts[DST_NODES_NUM];
    return new Message(src, dsts);
}

void Event::forwardMsg(Message &s) {
    if (s.begin_trans > 0) {
        s.begin_trans--;
        return;
    }

    if (s.begin_trans <= 0)
        s.count++;

    vector<NodeInfo> next = route->forward(s);
    int i = 1;
    vector<NodeInfo> temp1, temp2;
    temp2 = s.rpath[i - 1];
    while (i < MESSAGE_LENGTH) {
        temp1 = s.rpath[i];
        s.rpath[i] = temp2;
        temp2 = temp1;
        i++;
    }
    vector<NodeInfo> last_infos = s.rpath[MESSAGE_LENGTH-1];
    //  判断尾flit可以释放的buffer，即在temp2中但不在last_infos中
    for (auto it=temp2.begin(); it!=temp2.end(); ++it) {
        bool find = false;
        int temp_cur = it->cur;
        for (auto last_it=last_infos.begin(); last_it!=last_infos.end(); ++it) {
            if (temp_cur == last_it->cur) {
                find = true;
                break;
            }
        }
        if (!find && it->buffer != NULL) {
            it->buffer->bufferPlus(MESSAGE_LENGTH);
        }
    }
    s.rpath[0] = next;

    for (auto last_it=last_infos.begin(); last_it!=last_infos.end(); ++last_it) {
        if (last_it->dsts.empty()) { //  某个尾flit到达终点
            last_it->buffer->bufferPlus(MESSAGE_LENGTH);
            int cur = last_it->cur;
            for (int i=0; i<MESSAGE_LENGTH; ++i) {
                for (auto temp_it=s.rpath[i].begin(); temp_it!=s.rpath[i].end(); ) {
                    if (temp_it->cur == cur) {
                        s.rpath[i].erase(temp_it);
                    } else {
                        ++temp_it;
                    }
                }
            }
        }
    }

    if (s.rpath[MESSAGE_LENGTH-1].size() == 0) {
        //  组播结束
        s.finish = true;

        total_circle += s.count;
        message_completion_num += 1;
        if (s.fault_delivery == 0) {
            message_success_num += 1;
        }
        total_success_delivery += DST_NODES_NUM - FAULT_NODES_NUM;
    }

    return;
}
