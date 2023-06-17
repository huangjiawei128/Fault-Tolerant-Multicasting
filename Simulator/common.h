#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <assert.h>
#include <string>
#include <vector>

using namespace std;

//  模拟器参数配置
#define N_CUBE 6
#define MESSAGE_LENGTH 16
#define BUFFER_SIZE 64
#define FAULT_NODES_NUM 25
#define DST_NODES_NUM 10
#define PROCESS_TIME 16

#define TOTAL_CYCLE 1500
#define MSG_THRESHOLD 400

//  将二进制字符串s转为整数
int binary_str_to_int(string s);

//  将整数i转为长度为n的二进制字符串
string int_to_binary_str(int i, int n = N_CUBE);

//  '1'、'0'反转
char convert_01(char c);

//  将二进制字符串str中位置为pos的字符'1'、'0'反转
string convert_01_by_pos(string str, int pos);

//  从集合nums中随机选取k个元素
template<typename T>
vector<T> random_select(vector<T> nums, int k);

//  从集合{0,1,...,n-1}中随机选取k个元素
vector<int> random_select(int n, int k);

#endif
