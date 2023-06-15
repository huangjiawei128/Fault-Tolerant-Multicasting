#include <string>
#include <iostream>

using namespace std;

int main() {
    string s1 = "hjw";
    string s2 = "hjw";
    cout << (s1 == s2) << endl;
    s2[2] = 'v';
    cout << s1 << endl;
    cout << s2 << endl;
    return 0;
}