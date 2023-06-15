#ifndef COMMON_H
#define COMMON_H

#include <string>
using namespace std;

#define N_CUBE 6
#define MESSAGE_LENGTH 16
#define BUFFER_SIZE 64
#define DST_NODES_NUM 10

int binary_str_to_int(string s);
string int_to_binary_str(int i);
char convert_01(char c);

#endif
