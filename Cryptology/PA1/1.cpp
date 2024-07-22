#include<bits/stdc++.h>
using namespace std;

vector<int> possibleChars;

void getChars() {
    for (int i=0;i<26;i++) {
        possibleChars.push_back('a'+i);
        possibleChars.push_back('A'+i);
    }
    possibleChars.push_back(' ');
    possibleChars.push_back(',');
    possibleChars.push_back('.');
    possibleChars.push_back('\'');
    possibleChars.push_back('?');
    possibleChars.push_back('!');
    // digits
    for (int i=0;i<10;i++) {
        possibleChars.push_back('0'+i);
    }
    return;
}

bool checkEnglish(char m) {
    // check if m is in possible chars
    for (auto& c:possibleChars) {
        if (c == m) return true;
    }
    return false;
}

void getXORs(unordered_map<uint8_t, vector<pair<char, char>>>& m) {
    for (int i=0;i<possibleChars.size();i++) {
        for (int j=i;j<possibleChars.size();j++) {
            uint8_t ch1 = possibleChars[i], ch2 = possibleChars[j];
            m[ch1^ch2].push_back({ch1, ch2});
        }
    }
    return;
}

bool check(unordered_map<uint8_t, vector<pair<char, char>>>& m, uint8_t xr, char ch) {
    for (auto& p:m[xr]) {
        if (p.first == ch || p.second == ch) return true;
    }
    return false;
}

void printMessage(vector<set<char>>& v, int i) {
    // Take one char from each set, form all possible messages and print them
    if (i == v.size()) cout << '\n';
    else {
        for (auto& ch:v[i]) {
            cout << ch;
            printMessage(v, i+1);
        }
    }
    return;
}

int main() {
    getChars();
    unordered_map<uint8_t, vector<pair<char, char>>> mp;
    getXORs(mp);

    for (auto& it:mp) {
        // cout << (int)it.first << " " << it.second.size() << endl;
    }

    int n;
    n = 12;
    string ciphertexts[n];
    // Read n strings from streamciphertexts.txt
    ifstream fin("streamciphertexts.txt");
    for (int i = 0; i < n; i++) {
        fin >> ciphertexts[i];
    }
    fin.close();
    int keyLen = 0;
    for (int i=0;i<n;i++) {
        // cout << ciphertexts[i].length() << endl;
        keyLen = max(keyLen, (int)ciphertexts[i].length());
    }
    // cout << keyLen << endl;
    vector<uint8_t> encrKey(keyLen, 0);
    vector<set<char>> v;
    int possibleFirstMessages = INT_MAX;
    // string firstMessage = "";
    long long totalPoss = 1;

    for (int ind=0;ind<ciphertexts[0].length();ind++) {
        set<char> poss(possibleChars.begin(), possibleChars.end());
        for (int j=1;j<n;j++) {
            if (ind < ciphertexts[j].length()) {
                set<char> tempPoss = poss;
                uint8_t xr = ciphertexts[0][ind] ^ ciphertexts[j][ind];
                for (auto& ch:poss) {
                    if (!check(mp, xr, ch)) {
                        tempPoss.erase(ch);
                    }
                }
                poss = tempPoss;
            }
        }
        cout << poss.size() << endl;
        v.push_back(poss);
        totalPoss *= (long long)poss.size();
        possibleFirstMessages = min(possibleFirstMessages, (int)poss.size());
        // if (!poss.empty()) firstMessage += *poss.begin();
        // encrKey[ind] = *poss.begin();
    }
    // cout << firstMessage << endl;
    cout << possibleFirstMessages << endl;
    // printMessage(v, 0);
    cout << totalPoss << endl;

    // for (auto& k:encrKey) cout << (int)k << " ";
    // cout << endl;
    // First message
    for (int j=0;j<ciphertexts[0].length();j++) {
        // cout << (char)(encrKey[j] ^ ciphertexts[0][j]);
    }
    // cout << endl;
    for (int i=0;i<n;i++) {
        for (int j=0;j<ciphertexts[i].length();j++) {
            // cout << (char)(encrKey[j] ^ ciphertexts[i][j]);
        }
        // cout << endl;
    }
    return 0;
}