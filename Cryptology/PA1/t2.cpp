#include<bits/stdc++.h>
using namespace std;

int main() {
    ifstream fin("key.txt");
    vector<uint8_t> key;
    uint8_t temp;
    for (int i=0;i<140;i++) {
        fin >> temp;
        key.push_back(temp);
    }
    fin.close();
    for (int i=0;i<key.size();i++) {
        cout << i << " " << (int)key[i] << "\n";
    }
    // cout << key[129];
    cout << endl;
    return 0;
}