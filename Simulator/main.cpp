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
    int n_cube = N_CUBE;
    int buffer_size = BUFFER_SIZE;
    int nodes_num = pow(2,n_cube);
    int fault_nodes_num = FAULT_NODES_NUM;
    int dst_nodes_num = DST_NODES_NUM;
    Cube *cube = NULL;  //  网络结构
    Event *event = NULL;
    vector<int> fault_nodes_digit_ids = random_select(nodes_num, fault_nodes_num);

    /************************************************************************************

                    start simulate

    ************************************************************************************/
    double max_throughput = 0;

    //  link_rate控制消息产生速率
    for (float link_rate = 0.01; link_rate < 1;) {
        cube = new Cube(n_cube, buffer_size);   //  初始化网络结构
        route = new Routing(cube);
        event = new Event(route);
        event->setFaultNodes(fault_nodes_digit_ids);

        float msg_per_cir = link_rate * cube->getNodesNum();
        float flit_per_cir = msg_per_cir * MESSAGE_LENGTH;

        vector<Message *> messages;
        float k = 0;
        int msg_num = 0;

        /************************************************************************************

                        genarate message

        ************************************************************************************/
        for (int i = 0; i < total_circle && msg_num < threshold; i++) {
            for (k += msg_per_cir; k > 0; k--) {
                msg_num++;  //  已产生的总消息数加一
                messages.push_back(event->genMsg(dst_nodes_num));
            }

            /************************************************************************************

                            release link

            ************************************************************************************/
            for (vector<Message *>::iterator it = vecmess.begin(); it != vecmess.end(); it++) {

                /* if the tail of a message shifts ,
                the physical link the message occupied should release.
                  */

                if ((*it)->releaselink == true) {
                    assert((*it)->routpath[MESSAGE_LENGTH - 1].buff->link_used);
                    (*it)->routpath[MESSAGE_LENGTH - 1].buff->link_used = false;//释放链路
                    (*it)->releaselink = false;
                }
            }

            /************************************************************************************

                            forward message

            ************************************************************************************/
            for (vector<Message *>::iterator it = messages.begin(); it != messages.end();) {
                if ((*it)->active == false) {
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
        double latency = event->total_circle / event->message_completion_num;
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
            break;
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
