#include <string>
#include <vector>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <fstream>

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
    for (int i = 0; i < k; ++i) {
        int idx = abs(rand()) % (n - i);
        ret.push_back(nums[idx]);
        nums.erase(nums.begin() + idx);
    }
    return ret;
}

vector<int> random_select(int n, int k) {
    assert(k <= n);
    vector<int> nums;
    for (int i = 0; i < n; ++i) {
        nums.push_back(i);
    }
    return random_select(nums, k);
}

char convert_01(char c) {
    if (c == '0')
        return '1';
    return '0';
}

vector<string> getNodesInSubcube(string subcube) {
    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < subcube.size(); ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < pow(2, count); ++i) {
        string node_id = subcube;
        string s = int_to_binary_str(i, count);
        for (int j = 0; j < count; ++j) {
            int posi = unfixed_posis[j];
            node_id[posi] = s[j];
        }
        ret.push_back(node_id);
    }
    return ret;
}

bool isNodeInSubcube(string node_id, string subcube) {
    if (subcube.length() != 6) {
        return false;
    }
    for (int i = 0; i < 6; ++i) {
        if (subcube[i] != '*' && node_id[i] != subcube[i]) {
            return false;
        }
    }
    return true;
}

vector<string> getNodeNeighsInSubcube(string node_id, string subcube) {
    if (!isNodeInSubcube(node_id, subcube)) {
        return {};
    }
    if (subcube.length() != 6) {
        subcube = "******";
    }

    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < subcube.size(); ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < count; ++i) {
        string neigh_id = node_id;
        int posi = unfixed_posis[i];
        neigh_id[posi] = convert_01(neigh_id[posi]);
        ret.push_back(neigh_id);
    }
    return ret;
}

vector<string> getSubcubeChildren(string subcube) {
    vector<string> ret;
    vector<int> unfixed_posis;
    for (int i = 0; i < subcube.size(); ++i) {
        if (subcube[i] == '*') {
            unfixed_posis.push_back(i);
        }
    }
    int count = unfixed_posis.size();
    for (int i = 0; i < count; ++i) {
        int posi = unfixed_posis[i];
        string child_subcube = subcube;
        child_subcube[posi] = '0';
        ret.push_back(child_subcube);
        child_subcube[posi] = '1';
        ret.push_back(child_subcube);
    }

    return ret;
}

int main() {
    vector<int> ret = random_select(20, 8);
    for (int i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    ret = random_select(20, 8);
    for (int i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    ret = random_select(20, 8);
    for (int i = 0; i < ret.size(); ++i) {
        cout << ret[i] << endl;
    }

    cout << "----------" << endl;

    for (int i = 0; i < 64; ++i) {
        cout << int_to_binary_str(i, 6) << endl;
    }

    cout << "----------" << endl;

    cout << binary_str_to_int("000000") << endl;
    cout << binary_str_to_int("000100") << endl;
    cout << binary_str_to_int("010100") << endl;

    cout << "----------" << endl;

    unordered_map<int, vector<string>> delivery_record;
    for (int i = -1; i < 6; ++i) {
        delivery_record.insert(make_pair(i, vector<string>{}));
    }
    (delivery_record.find(3)->second).push_back("abc");
    (delivery_record.find(3)->second).push_back("def");
    for (int i = 0; i < delivery_record[3].size(); ++i)
        cout << delivery_record[3][i] << endl;

    cout << "----------" << endl;

    vector<string> r1 = getNodesInSubcube("1*1**0");
    for (auto it = r1.begin(); it != r1.end(); ++it) {
        cout << *it << endl;
    }

    cout << "----------" << endl;

    vector<string> r2 = getNodeNeighsInSubcube("111110", "1*1**0");
    for (auto it = r2.begin(); it != r2.end(); ++it) {
        cout << *it << endl;
    }

    cout << "----------" << endl;

    vector<string> r3 = getSubcubeChildren("1*1**0");
    for (auto it = r3.begin(); it != r3.end(); ++it) {
        cout << *it << endl;
    }

    cout << "----------" << endl;
    ofstream out;
    string file_path = "../result/fault_nodes_num=";
    file_path = file_path + to_string(20);
    out.open(file_path);
    out << "123" << endl;
    out.close();

    return 0;
}