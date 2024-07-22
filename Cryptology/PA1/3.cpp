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

// Check value of others if m[msg][i] is a space character
void checkOthers(vector<string>& ciphertexts, int msg, int i) {
    cout << "Checking when m" << msg << "_" << i << " is space character\n";
    for (int j=0;j<ciphertexts.size();j++) {
        if (j == msg) continue;
        if (i >= ciphertexts[j].length()) continue;
        uint8_t ch = ciphertexts[msg][i] ^ ciphertexts[j][i] ^ ' ';
        // if ((ch >= 'a' and ch <= 'z') or (ch >= 'A' and ch <= 'Z')) {
        //     cout << "m" << j << "_" << i << " will be " << (char)ch << '\n';
        // }
        if (checkEnglish(ch)) {
            cout << "m" << j << "_" << i << " will be \"" << (char)ch << "\"\n";
        }
    }
}

int main() {
    getChars();
    // unordered_map<uint8_t, vector<pair<char, char>>> mp;
    // getXORs(mp);

    // for (auto& it:mp) {
    //     // cout << (int)it.first << " " << it.second.size() << endl;
    // }

    int n;
    n = 12;
    vector<string> ciphertexts(n);
    // Read n strings from streamciphertexts.txt
    ifstream fin("streamciphertexts.txt");
    for (int i = 0; i < n; i++) {
        fin >> ciphertexts[i];
    }
    fin.close();
    int keyLen = 0;
    for (int i=0;i<n;i++) {
        cout << ciphertexts[i].length() << endl;
        keyLen = max(keyLen, (int)ciphertexts[i].length());
    }

    // int firstMsg = 0, secondMsg = 1;
    // int len = min(ciphertexts[firstMsg].length(), ciphertexts[secondMsg].length());
    // for (int i=0;i<len;i++) {
    //     uint8_t xr = ciphertexts[firstMsg][i]^ciphertexts[secondMsg][i];
    //     if ((xr >= 'a' and xr <= 'z') or (xr >= 'A' and xr <= 'Z')) {
    //         cout << "Found m" << firstMsg << "_" << i << " XOR m" << secondMsg << "_" << i << " equal to " << (char)xr << '\n';
    //         checkOthers(ciphertexts, firstMsg, i);
    //         checkOthers(ciphertexts, secondMsg, i);
    //         cout << '\n';
    //     }
    // }
    return 0;
}