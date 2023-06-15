#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <assert.h>
#include <string>
#include <vector>

using namespace std;

#define N_CUBE 6
#define MESSAGE_LENGTH 16
#define BUFFER_SIZE 64
#define FAULT_NODES_NUM 5
#define DST_NODES_NUM 10

int binary_str_to_int(string s);

string int_to_binary_str(int i);

char convert_01(char c);

template<typename T>
vector<T> random_select(vector<T> nums, int k);

vector<int> random_select(int n, int k);

#endif
