#include"Event.h"

Event::Event(AllRouting *route) {
    total_circle = 0;
    message_completion_num = 0;
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

Message *Event::genMsg(int dst_nodes_num) {  // generate a message
    Cube *cube = NULL;
    if (route != NULL)
        cube = route->cube;
    //  采用uniform流量模式
    vector<int> dsts = random_select(normal_nodes_digit_ids, dst_nodes_num + 1);
    int src = dsts[dst_nodes_num];
    return new Message(src, dsts);
}

void Event::forwardMsg(Message &s) {
    if (s.begintrans <= 0) s.count++;
    if (s.routpath[0].node == s.src) {
        if (s.begintrans > 0)
            s.begintrans--; // check whether process time
        else {
            s.begintrans--;
            NodeInfo *next;
            assert(rout != NULL);
            next = rout->forward(s);
            if (next->node == -1) {
                s.timeout++;
//						if(s.timeout == TIMEOUT) consumeMes(s);
            } else {
                s.timeout = 0;
                assert(s.routpath[0].node != next->node);
                s.routpath[0] = *next;

            }


        }
    } else {
        if (s.routpath[0].node != s.dst) {
            NodeInfo *next = NULL;
            if (rout != NULL)
                next = rout->forward(s);//调用AllRouting.h中的forward()函数，得到header flit下一跳的节点
            if (next->node == -1) {
                int i;
                for (i = 1; i < MESSAGE_LENGTH && s.routpath[i].node ==
                                                  s.routpath[0].node; i++); //if blocked insert the flits until all are inserted
                if (i < MESSAGE_LENGTH) {
                    NodeInfo temp1, temp2; // all the flits forward one step
                    temp2 = s.routpath[i - 1];
                    while (i < MESSAGE_LENGTH) {
                        temp1 = s.routpath[i];
                        s.routpath[i] = temp2;
                        temp2 = temp1;
                        i++;
                    }
                    if (temp2.node != s.routpath[MESSAGE_LENGTH - 1].node) {
                        assert(s.routpath[MESSAGE_LENGTH - 1].buff->link_used);
                        s.releaselink = true;
                    }
                    if (temp2.buff != NULL && temp2.node != s.routpath[MESSAGE_LENGTH - 1].node)
                        temp2.buff->bufferPlus(temp2.channel, MESSAGE_LENGTH);

                }
                s.timeout++;
                //	if(s.timeout == TIMEOUT) consumeMes(s);
            } else {
                s.timeout = 0;
                NodeInfo temp1, temp2; // all the flits forward one step
                temp2 = s.routpath[0];
                int i = 1;
                while (i < MESSAGE_LENGTH) {
                    temp1 = s.routpath[i];
                    s.routpath[i] = temp2;
                    temp2 = temp1;
                    i++;
                }
                if (temp2.node != s.routpath[MESSAGE_LENGTH - 1].node) {
                    assert(s.routpath[MESSAGE_LENGTH - 1].buff->link_used);
                    s.releaselink = true;   // the tail shift, release the physical link
                }
                if (temp2.buff != NULL &&
                    temp2.node !=
                    s.routpath[MESSAGE_LENGTH - 1].node) // maybe the last flit do not insert into the network
                    temp2.buff->bufferPlus(temp2.channel, MESSAGE_LENGTH);

                s.routpath[0] = *next;


            }
        } else {
            NodeInfo temp1, temp2; // all the flits forward one step
            temp2 = s.routpath[0];
            int i;
            for (i = 1; i < MESSAGE_LENGTH && s.routpath[i].node == s.routpath[0].node; i++);
            if (i == MESSAGE_LENGTH) {
                s.routpath[i - 1].buff->bufferPlus(s.routpath[i - 1].channel, MESSAGE_LENGTH); // add buffer
                s.active = false;
                totalcir += s.count;
                messarrive++;
                return;
            }
            while (i < MESSAGE_LENGTH) {
                temp1 = s.routpath[i];
                s.routpath[i] = temp2;
                temp2 = temp1;
                i++;
            }
            if (temp2.node != s.routpath[MESSAGE_LENGTH - 1].node)
                s.releaselink = true;   // the tail shift, release the physical link

            if (temp2.buff != NULL &&
                temp2.node != s.routpath[MESSAGE_LENGTH - 1].node) // maybe the last flit do not insert
                temp2.buff->bufferPlus(temp2.channel, MESSAGE_LENGTH);


        }


    }
    //cout << s.src << "dst: " << s.dst << endl ;

}
