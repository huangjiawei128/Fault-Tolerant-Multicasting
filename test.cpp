#include <string>
#include <vector>
#include <math.h>
#include <assert.h>
#include <iostream>

using namespace std;

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
    return 0;
}