#include "Cube.h"

Cube::Cube(int n, int buffer_size) {
    this->n = n;
    this->buffer_size = buffer_size;
    nodes_num = pow(2,n);
    head = new CubeNode[nodes_num];
    assert(head);

    for (int i=0; i<nodes_num; ++i) {
        (head + i)->Initialize(i);
        (head + i)->setCube(this);
        (head + i)->setBuffer(buffer_size);
    }

    for (int i=0; i<nodes_num; ++i) {
        (head + i)->setLinkBuffer();
    }
}

void Cube::setFault(string id) {
    (*this)[id]->fault = true;
}

void Cube::setFault(int digit_id) {
    (*this)[digit_id]->fault = true;
}

CubeNode *Cube::operator[](string id) {
    return head + binary_str_to_int(id);
}

CubeNode *Cube::operator[](int digit_id) {
    return head + digit_id;
}

void Cube::clearAll() {
    for (int i = 0; i < pow(2,n); i++) {
        (head + i)->clearBuffer();
    }
}
