#include "common.h"
#include "Cube.h"
#include "Routing.h"
#include "Event.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream out;
    string file_path = "../result/fault_nodes_num=";
    file_path = file_path + to_string(FAULT_NODES_NUM) + ".csv";
    out.open(file_path);
    out << "link_rate,fail_percent,latency,throughput" << endl;

    Routing *route = NULL;
    int node_num = pow(2, N_CUBE);
    Cube *cube = NULL;
    Event *event = NULL;
    vector<int> fault_nodes_digit_ids = random_select(node_num, FAULT_NODES_NUM);   //  随机挑选故障节点编号
    bool end = false;

    /************************************************************************************

                    start simulate

    ************************************************************************************/
    double max_throughput = 0;

    //  link_rate控制消息产生速率
    for (float link_rate = ORI_LINK_RATE; link_rate < 1 && !end;) {
        cube = new Cube(N_CUBE, BUFFER_SIZE);   //  初始化网络拓扑模块
        route = new Routing(cube);  //  初始化路由模块
        event = new Event(route);   //  初始化事件模块
        event->setFaultNodes(fault_nodes_digit_ids);    //  设置故障节点
        event->setMscsForCube();    //  生成网络拓扑的局部信息（容错算法中使用）

        float msg_per_cir = link_rate * node_num;   //  link rate(message/node/cycle)表示单位时间内每个节点产生的消息数

        vector<Message *> messages;
        float k = 0;
        int msg_num = 0;

        /************************************************************************************

                        genarate message

        ************************************************************************************/
        for (int i = 0; i < TOTAL_CYCLE && msg_num < MSG_THRESHOLD; i++) {
            for (k += msg_per_cir; k > 0; k--) {
                msg_num++;
                messages.push_back(event->genMsg());    //  调用事件模块生成消息
            }

            /************************************************************************************

                            release link

            ************************************************************************************/
            for (vector<Message *>::iterator it = messages.begin(); it != messages.end(); it++) {
                //  在尾flit移动的场景下释放先前所占据的链路
                vector<NodeInfo> last_node_infos = (*it)->rpath[MESSAGE_LENGTH - 1];
                for (auto info_it = last_node_infos.begin(); info_it != last_node_infos.end(); ++info_it) {
                    Buffer *buffer = info_it->buffer;
                    if (buffer != NULL && buffer->link_used) {
                        buffer->link_used = false;
                    }
                }
            }

            /************************************************************************************

                            forward message

            ************************************************************************************/
            for (vector<Message *>::iterator it = messages.begin(); it != messages.end();) {
                if ((*it)->finish == true) {
                    delete (*it);
                    it = messages.erase(it);    //  消息完成组播，将它从消息列表中删除
                } else
                    event->forwardMsg(*(*it++));    //  调用事件模块在当前cycle内以flit为单位转发消息
            }
        }

        /*****************************************************************************

                        output results

        *****************************************************************************/
        int size = messages.size();
        double fail_percent = 1 - (double)event->message_success_num / event->message_completion_num;
        //  fail percent = 1 - 成功完成组播的消息数 / 完成组播的消息数
        double latency = (float) event->total_cycle / event->message_completion_num;
        //  latency(cycle) = 完成组播的消息消耗的总cycle数 / 完成组播的消息数
        double throughput = link_rate * ((float) event->message_success_num / msg_num);
        //  throughput(message/node/cycle) = link_rate * (成功完成组播的消息数 / 已产生的总消息数)

        cout << endl << endl << "link rate: " << link_rate << "     complete: " << event->message_completion_num
             << "     in the network: " << size << "    fail percent: " << fail_percent << endl
             << "latency: " << latency << "     throughput: " << throughput
             << endl << endl;

        out << link_rate << "," << fail_percent << ","
            << latency << "," << throughput << endl;

        /************************************************************************************

                        whether arrive at saturation point

        *************************************************************************************/
        //  throughput达到最大值，即饱和点时，停止迭代
        if (throughput > max_throughput)
            max_throughput = throughput;
        else {
            cout << "max_throughput: " << max_throughput << endl;
            end = true;
        }

        /************************************************************************************

                        clean

        ************************************************************************************/
        for (auto it = messages.begin(); it != messages.end(); ++it) {
            delete *it;
        }
        delete route;
        delete cube;
        delete event;

        link_rate += INC_LINK_RATE;
    }

    out.close();

    return 1;
}
