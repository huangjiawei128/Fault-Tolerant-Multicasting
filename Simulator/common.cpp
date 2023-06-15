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

char convert(char c) {
    if (c == '0')
        return '1';
    return '0';
}
