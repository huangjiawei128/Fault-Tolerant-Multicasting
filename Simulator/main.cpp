#include "common.h"
#include "Cube.h"
#include "AllRouting.h"
#include "Event.h"

int main() {
    int total_circle = 100000;
    int threshold = 800;
    AllRouting *route = NULL;
    int n_cube = N_CUBE;
    int buffer_size = BUFFER_SIZE;
    Cube *cube = NULL;  //  网络结构
    Event *s = NULL;

    /************************************************************************************

                    start simulate

    ***********************************************************************************/
    //  link_rate控制消息产生速率
    int link_rate;
    for (link_rate = 0.01; link_rate < 1;) {
        cube = new Cube(n_cube, buffer_size); //初始化网络结构
        route = new Routing(cube);
        s = new Event(route);


        float msgpercir = (float) (link_rate * 2 * 2 * knode /
                                   (MESSAGE_LENGTH * 10));//每个周期每个节点产生的message数，还要除以10是因为allvecmess有10个元素
        //saturationRate = (double)(knode * 2 * 2) / (double)(knode * knode); 在mesh网络中的饱和吞吐量
        //msgpercir = link_rate * saturationRate * knode * knode; 每个周期每个节点产生的flit数

        vector<Message *> allvecmess[10];
        float k = 0;
        int allmess = 0;



        /************************************************************************************

                      genarate message

    ***********************************************************************************/
        //执行total_circle个周期，getsize(allvecmess) < threshold只是自己加的限制条件，可以有也可以删除，具体的threshold和total_circle值也可以在前面修改
        for (int i = 0; i < total_circle && getsize(allvecmess) < threshold; i++) {
            vector<Message *> &vecmess = allvecmess[i % 10];
            for (k += msgpercir; k > 0; k--) {
                allmess++;//总的产生消息数加一
                vecmess.push_back(s->genMes());//产生消息放入	allvecmess的某个元素中
            }


            /************************************************************************************

                            release link

          ***********************************************************************************/


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

          ***********************************************************************************/
            for (vector<Message *>::iterator it = vecmess.begin(); it != vecmess.end();) {
                if ((*it)->active == false) {      // when a message arrive at its destination, it is not active
                    delete (*it);
                    it = vecmess.erase(it);//消息到达目的节点，将它从vecmess中删除
                } else
                    s->forwardMes(*(*it++));//调用Routing.cpp中函数转发消息
            }
        }




        /*****************************************************************************

                        output results
          ****************************************************************************/

        int size = getsize(allvecmess);


        //s->totalcir/s->messarrive 平均延迟的计算公式；link_rate * ((float) s->messarrive / allmess)吞吐量的计算公式
        cout << endl << endl << "link_rate:" << link_rate << "    arrive:  " << s->messarrive
             << "    in the network : "
             << size << endl << "average latency: "
             << (s->totalcir / s->messarrive) << "  nomalized accepted traffic: "
             << link_rate * ((float) s->messarrive / allmess)
             << endl << endl;

        out << link_rate * ((float) s->messarrive / allmess)
            << " " << (s->totalcir / s->messarrive) << endl;





        /************************************************************************************

                            whether arrive at saturation point

      ***********************************************************************************/
        if (link_rate * ((float) s->messarrive / allmess) > max
            && ((link_rate * ((float) s->messarrive / allmess) - max) / max) > 0.01
            && getsize(allvecmess) < threshold)
            max = link_rate * ((float) s->messarrive / allmess);


        else {
            cout << "Saturation point, drain......." << endl;
            drain(allvecmess, cube, s);
            int size = 0;
            for (int j = 0; j < 10; j++) {
                if (!allvecmess[j].empty()) {
                    size += allvecmess[j].size();
                }
            }
            cout << "in the network:      " << size << endl;
            outtotest(allvecmess, cube);
            bufferleft(cube, knode);
            cout << "max:" << max << endl;
            break;
        }





        /************************************************************************************
                        clean
      *******************************************************************************************/


        for (int m = 0; m < 10; m++) {
            for (vector<Message *>::iterator it = allvecmess[m].begin();
                 it != allvecmess[m].end(); it++)
                delete (*it);
        }
        delete route;
        delete cube;
        delete s;


        if (link_rate < 0.5)
            link_rate += 0.05;
        else
            link_rate += 0.02;
    }

    return 1;
}


