#ifndef CUBE_H
#define CUBE_H

#include "CubeNode.h"
#include "common.h"
#include <math.h>
#include <assert.h>
#include <string>
using namespace std;

class CubeNode;

class Cube {
private:
    int n;
    int node_num;
    int buffer_size;
    CubeNode *head;

public:
    Cube(int n, int buffer_size);    //  n表示Cube的维数
    CubeNode *operator[](string id);
    CubeNode *operator[](int digit_id);
    void setFault(string id);
    void setFault(int digit_id);
    void clearAll();
    int getDimensionsNum() { return n; }
    int getNodeNum() {return node_num; }

    ~Cube() {
        delete[] head;
    }
};

#endif