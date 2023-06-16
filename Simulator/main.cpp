#include "common.h"
#include "Cube.h"
#include "AllRouting.h"
#include "Event.h"
#include <math.h>
#include <vector>

using namespace std;

int main() {
    int total_circle = 100000;
    int threshold = 800;
    AllRouting *route = NULL;
    int nodes_num = pow(2,N_CUBE);
    Cube *cube = NULL;  //  网络结构
    Event *event = NULL;
    vector<int> fault_nodes_digit_ids = random_select(nodes_num, FAULT_NODES_NUM);
    bool end = false;

    /************************************************************************************

                    start simulate

    ************************************************************************************/
    double max_throughput = 0;

    //  link_rate控制消息产生速率
    for (float link_rate = 0.01; link_rate < 1 && !end;) {
        cube = new Cube(N_CUBE, BUFFER_SIZE);   //  初始化网络结构
        route = new Routing(cube);
        event = new Event(route);
        event->setFaultNodes(fault_nodes_digit_ids);

        float msg_per_cir = link_rate * nodes_num;

        vector<Message *> messages;
        float k = 0;
        int msg_num = 0;

        /************************************************************************************

                        genarate message

        ************************************************************************************/
        for (int i = 0; i < total_circle && msg_num < threshold; i++) {
            for (k += msg_per_cir; k > 0; k--) {
                msg_num++;  //  已产生的总消息数加一
                messages.push_back(event->genMsg());
            }

            /************************************************************************************

                            release link

            ************************************************************************************/
            for (vector<Message *>::iterator it = messages.begin(); it != messages.end(); it++) {
                /* if the tail of a message shifts ,
                the physical link the message occupied should release.
                */
                vector<NodeInfo> last_node_infos = (*it)->rpath[MESSAGE_LENGTH-1];
                for (auto info_it=last_node_infos.begin(); info_it!=last_node_infos.end(); ++info_it) {
                    Buffer* buffer = info_it->buffer;
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
                    it = messages.erase(it);    //  消息完成组播，将它从messages中删除
                } else
                    event->forwardMsg(*(*it++));    //  调用Routing.cpp中函数转发消息
            }
        }

        /*****************************************************************************

                        output results

        *****************************************************************************/
        int size = messages.size();
        double latency = (float) event->total_circle / event->message_completion_num;
        double throughput = link_rate * ((float) event->message_success_num / msg_num);

        cout << endl << endl <<"link_rate: " << link_rate << "     complete: " << event->message_completion_num
             << "     in the network: " << size << endl
             << "average latency: " << latency << "     throughput: " << throughput
             << endl << endl;

        /************************************************************************************

                        whether arrive at saturation point

        *************************************************************************************/
        if (throughput > max_throughput
            && (throughput - max_throughput) / max_throughput > 0.01
            && size < threshold)
            max_throughput = throughput;
        else {
            cout << "max_throughput: " << max_throughput << endl;
            end = true;
        }

        /************************************************************************************

                        clean

        ************************************************************************************/
        for (auto it=messages.begin(); it != messages.end(); ++it) {
            delete *it;
        }
        delete route;
        delete cube;
        delete event;

        if (link_rate < 0.5)
            link_rate += 0.05;
        else
            link_rate += 0.02;
    }

    return 1;
}
