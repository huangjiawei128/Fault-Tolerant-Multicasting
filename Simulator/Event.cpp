#include"Event.h"

Event::Event(Routing *route) {
    this->total_cycle = 0;
    this->message_completion_num = 0;
    this->message_success_num = 0;
    this->route = route;
    this->cube = route->cube;
    this->n = route->n;
}

void Event::setFaultNodes(vector<int> fault_nodes_digit_ids) {
    this->fault_nodes_digit_ids = fault_nodes_digit_ids;
    for (auto it = fault_nodes_digit_ids.begin(); it != fault_nodes_digit_ids.end(); ++it) {
        cube->setFault(*it);
    }
    for (int i = 0; i < cube->getNodeNum(); ++i) {
        if (find(fault_nodes_digit_ids.begin(), fault_nodes_digit_ids.end(), i) ==
            fault_nodes_digit_ids.end()) {
            this->normal_nodes_digit_ids.push_back(i);
        }
    }
    assert(fault_nodes_digit_ids.size() + normal_nodes_digit_ids.size() == cube->getNodeNum());
}

void Event::setMscsForCube() {
    cube->setMscs();
}

Message *Event::genMsg() {
    vector<int> dsts = random_select(normal_nodes_digit_ids, DST_NODES_NUM + 1);
    int src = dsts.back();
    dsts.pop_back();
    Message *ret = new Message(src, dsts);
    return ret;
}

void Event::forwardMsg(Message &s) {
    //  若消息从生成至开始传输的等待期未结束，则不进行传输
    if (s.begin_trans > 0) {
        s.begin_trans--;
        return;
    }

    if (s.begin_trans <= 0)
        s.count++;

    //  若某个尾flit到达目标节点/无法决策下一跳的节点，在所有flit的所在节点信息列表中删除该节点信息，并恢复消息占据的buffer容量
    for (auto last_it = s.rpath[MESSAGE_LENGTH - 1].begin(); last_it != s.rpath[MESSAGE_LENGTH - 1].end();) {
        if (last_it->dsts.empty()) {
            if (last_it->buffer != NULL)
                last_it->buffer->bufferPlus(MESSAGE_LENGTH);
            int cur = last_it->cur;
            for (int i = 0; i < MESSAGE_LENGTH - 1; ++i) {
                for (auto temp_it = s.rpath[i].begin(); temp_it != s.rpath[i].end(); ++temp_it) {
                    if (temp_it->cur == cur) {
                        s.rpath[i].erase(temp_it);
                        break;
                    }
                }
            }
            last_it = s.rpath[MESSAGE_LENGTH - 1].erase(last_it);
        } else {
            ++last_it;
        }
    }

    if (s.rpath[MESSAGE_LENGTH - 1].size() == 0) {  //  组播已结束，更改消息模块的某些统计值
        s.finish = true;

        total_cycle += s.count;
        message_completion_num += 1;
        if (s.fault_delivery == 0) {
            message_success_num += 1;
        }
    } else {    //  组播未结束
        //  所有非首flit的所在节点信息列表更新为上一cycle其前面flit的所在节点信息列表
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
        s.rpath[0] = next;

        //  若有尾flit发生移动，则其移动前消息占据的buffer容量可恢复
        vector<NodeInfo> last_infos = s.rpath[MESSAGE_LENGTH - 1];
        for (auto temp_it = temp2.begin(); temp_it != temp2.end(); ++temp_it) {
            if (temp_it->buffer == NULL)
                continue;

            bool find = false;
            int temp_cur = temp_it->cur;
            for (auto last_it = last_infos.begin(); last_it != last_infos.end(); ++last_it) {
                if (temp_cur == last_it->cur) {
                    find = true;
                    break;
                }
            }
            if (!find) {
                temp_it->buffer->bufferPlus(MESSAGE_LENGTH);
            }
        }
    }

    return;
}
