#include "CubeNode.h"

void Buffer::bufferMin(int n) {
    r -= n;
}

void Buffer::bufferPlus(int n) {
    r += n;
}

void CubeNode::Initialize(Cube *cube, int digit_id) {
    this->cube = cube;
    this->n = cube->getDimensionsNum();
    this->digit_id = digit_id;
    this->id = int_to_binary_str(digit_id, n);
    this->fault = false;
}

void CubeNode::setBuffer(int buffer_size)
{
    for (int i=0; i<n; ++i) {
        buffers.push_back(new Buffer());
    }

    clearBuffer();

    for (int i=0; i<n; ++i) {
        buffers[i]->r = buffer_size;
    }
}

void CubeNode::setLinkBuffer() {
    for (int i=0; i<n; ++i) {
        string neigh_id = id;
        neigh_id[i] = convert_01(neigh_id[i]);
        link_buffers.push_back((*cube)[neigh_id]->buffers[i]);
    }
}

void CubeNode::bufferPlus(Buffer *buff, int n) {
    buff->bufferPlus(n);
}

void CubeNode::bufferMin(Buffer *buff, int n) {
    buff->bufferMin(n);
}

void CubeNode::clearBuffer() {
    for (int i=0; i<n; ++i) {
        buffers[i]->link_used = false;
    }
}
