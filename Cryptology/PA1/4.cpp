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

vector<uint8_t> xorStrings(vector<string>& ciphertexts, int m1, int m2, int i, string& checkString, int len) {
    vector<uint8_t> res;
    for (int j=0;j<len;j++) {
        res.push_back(ciphertexts[m1][i+j]^ciphertexts[m2][i+j]^checkString[j]);
    }
    return res;
}

bool checkEnglishString(vector<uint8_t>& v) {
    for (auto& ch:v) {
        if (!checkEnglish(ch)) return false;
    }
    return true;
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
        // cout << ciphertexts[i].length() << endl;
        keyLen = max(keyLen, (int)ciphertexts[i].length());
    }

    int msg;
    string checkString = "the";
    for (msg = 0; msg < n; msg++) {
        int lenMsg = ciphertexts[msg].length();
        int subLen = checkString.length();
        // write in {msg}.txt
        ofstream fout("msg" + to_string(msg) + ".txt");
        
        for (int i=0;i<=lenMsg - subLen; i++) {
            string sub = ciphertexts[msg].substr(i, subLen);
            fout << "m" << msg << "[" << i << ", " << i + subLen - 1 << "] equals \"" << checkString << "\"\n";
            int ct = 0;
            for (int j=0;j<n;j++) {
                if (j == msg) continue;
                if (i + subLen > ciphertexts[j].length()) continue;
                string sub2 = ciphertexts[j].substr(i, subLen);
                vector<uint8_t> xr = xorStrings(ciphertexts, msg, j, i, checkString, subLen);
                if (checkEnglishString(xr)) {
                    fout << "m" << j << "[" << i << ", " << i + subLen - 1 << "] will be \"";
                    for (auto& ch:xr) fout << (char)ch;
                    fout << "\"\n";
                    ct++;
                }
            }
            if (ct == 11) fout << "Good!!\n";
            fout << "\n";
        }
        fout.close();
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