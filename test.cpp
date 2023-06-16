#include <string>
#include <vector>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <unordered_map>

using namespace std;

int binary_str_to_int(string s) {
    return stoi(s, 0, 2);
}

string int_to_binary_str(int i, int n) {
    string ret = "";
    if (i == 0) {
        ret = "0";
    } else {
        while (i) {
            ret.insert(0, to_string(i % 2));
            i /= 2;
        }
    }
    int dis = n - ret.size();
    if (dis > 0) {
        ret = string(dis, '0') + ret;
    }
    return ret;
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


int main() {
    vector<int> ret = random_select(20, 8);
    for (int i=0; i<ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    ret = random_select(20, 8);
    for (int i=0; i<ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    ret = random_select(20, 8);
    for (int i=0; i<ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    for (int i=0; i<64; ++i) {
        cout << int_to_binary_str(i, 6) << endl;
    }

    cout << "----------" << endl;

    cout << binary_str_to_int("000000") << endl;
    cout << binary_str_to_int("000100") << endl;
    cout << binary_str_to_int("010100") << endl;

    cout << "----------" << endl;

    unordered_map<int, vector<string>> delivery_record;
    for (int i=-1; i<6; ++i) {
        delivery_record.insert(make_pair(i, vector<string>{}));
    }
    (delivery_record.find(3)->second).push_back("abc");
    (delivery_record.find(3)->second).push_back("def");
    for (int i=0; i<delivery_record[3].size(); ++i)
        cout << delivery_record[3][i] << endl;

    return 0;
}