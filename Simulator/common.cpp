#include "common.h"

int binary_str_to_int(string s) {
    return stoi(s, 0, 2);
}

string int_to_binary_str(int i) {
    string ret = "";
    if (i == 0) return "0";
    while (i) {
        ret.insert(0, to_string(i % 2));
        i /= 2;
    }
    return ret;
}

char convert_01(char c) {
    if (c == '0')
        return '1';
    return '0';
}

template<typename T>
vector<T> random_select(vector<T> nums, int k) {
    int n = nums.size();
    assert(k <= n);
    vector<T> ret;
    for (int i=0; i<k; ++i) {
        int idx = abs(rand()) % (n-i);
        ret.push_back(nums[idx]);
        nums.erase(nums.begin() + idx);
    }
    return ret;
}

vector<int> random_select(int n, int k) {
    assert(k <= n);
    vector<int> nums;
    for (int i=0; i<n; ++i) {
        nums.push_back(i);
    }
    return random_select(nums, k);
}
