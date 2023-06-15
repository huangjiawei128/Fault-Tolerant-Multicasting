#ifndef   EVEN_T
#define   EVEN_T

#include"Routing.h"
#include"Message.h"
//#include"AllMessage.h"
#include"Cube.h"
#include"CubeNode.h"


class Event {
public:
    int consumed;
    double totalcir; // total circle all the messages take;
    int messarrive; // how many message arrived
    AllRouting *rout;
    Cube *tor;
    int k;// k * k *k cube

    Message *genMes();          //generate a message
    void forwardMes(Message &);//forward a message
//void        consumeMes(Message&);
    Event(AllRouting *);
};


#endif