#include<bits/stdc++.h>
using namespace std;

// Print all the messages by XOR of the ciphertexts and the key
void printAllMsgs(vector<vector<uint8_t>>& ciphertext_vector, vector<uint8_t>& key) {
    for (int i=0;i<ciphertext_vector.size();i++) {
        cout << "m" << i+1 << " =  \"";
        for (int j=0;j<key.size();j++) {
            if (j >= ciphertext_vector[i].size()) {
                continue;
            }
            else {
                char ch = ciphertext_vector[i][j] ^ key[j];
                cout << ch;
            }
        }
        cout << "\"\n";
    }
    return;
}

// Convert ciphertexts from string format to 8 bit integer format
// And return the length of the key
int convert_to_8_bits(vector<string>& ciphertexts, vector<vector<uint8_t>>& ciphertext_vector) {
    int keyLen = 0;

    for (int i=0;i<ciphertexts.size();i++) {
        keyLen = max(keyLen, (int)ciphertexts[i].length());
        for (int j=0;j<ciphertexts[i].length();j+=2) {
            string temp = "";
            temp += ciphertexts[i][j];
            temp += ciphertexts[i][j+1];
            // Convert hexadecimal string temp to uint8_t
            uint8_t temp_int = stoi(temp, 0, 16);
            ciphertext_vector[i].push_back(temp_int);
        }
    }

    keyLen /= 2;

    return keyLen;
}

// Calculate the temporary key formed when we guess that guessedString is the next substring of the message
// Store the temporary key in temp_key
// Return a boolean indicating if the guessed string is out of bound for this message
bool getNewTemporaryKey(vector<vector<uint8_t>>& ciphertext_vector, vector<uint8_t>& previousKey, vector<uint8_t>& temp_key, string& guessedString, int msg, int currentGuessingIndex) {
    bool outOfBound = false; // True if the guessed string is crossing the bounds of the message
    
    // Create a copy of the previous key
    temp_key = previousKey;

    for (int j=0;j<guessedString.length();j++) {
        if (currentGuessingIndex + j >= ciphertext_vector[msg].size()) {
            cout << "This string is out of bound for this message\n\n";
            outOfBound = true;
            break;
        }

        // Concatenate the XOR of guessed string and ciphertext in the temporary key
        uint8_t k = ciphertext_vector[msg][currentGuessingIndex+j] ^ guessedString[j];
        temp_key.push_back(k);
    }

    return outOfBound;
}

// Provide the name of ciphertext file as command line argument
int main(int argc, char* argv[]) {

    // n = number of ciphertexts
    int n;
    n = 12;
    vector<string> ciphertexts(n);

    // Read n strings from ciphertext file
    ifstream fin(argv[1]);
    for (int i = 0; i < n; i++) {
        fin >> ciphertexts[i];
    }
    fin.close();    

    // ciphertext_vector[i] is the array of 8 bit integers representing the characters of ciphertext[i] in 8 bit integer format
    vector<vector<uint8_t>> ciphertext_vector(n);

    // Length of the key is equal to the length of longest message
    // which is half of the lenght of longest ciphertext
    int keyLen = convert_to_8_bits(ciphertexts, ciphertext_vector);

    // The key to be found
    vector<uint8_t> final_key;

    // currentGuessingIndex is the index at which we are guessing the string or character
    int currentGuessingIndex = 0;

    while (currentGuessingIndex < keyLen) {
        int msg;
        // Enter the message number in 1th index
        cout << "Enter message number: ";
        cin >> msg;
        msg--;

        if (currentGuessingIndex >= ciphertext_vector[msg].size()) {
            cout << "Message number " << msg + 1 << " is already decrypted\n\n";
            continue;
        }

        // Guessed string
        string guessedString;
        cout << "Enter guessed string: ";
        cin.ignore();
        getline(cin, guessedString);

        // Temporary key
        vector<uint8_t> temp_key;
        
        bool outOfBound = getNewTemporaryKey(ciphertext_vector, final_key, temp_key, guessedString, msg, currentGuessingIndex);

        if (outOfBound) continue;

        // Find out all the messages using the temporary key
        cout << "\nResulting messages:\n";
        printAllMsgs(ciphertext_vector, temp_key);

        // User will see if the resulting messages make sense
        // Accordingly, he/she will choose whether or not the guess was correct
        int correctGuess;
        cout << "\nContinue with this? (1/0): ";
        cin >> correctGuess;

        if (correctGuess > 0) {
            // If the guess was correct, update the final key and the current guessing index
            final_key = temp_key;
            currentGuessingIndex += guessedString.length();
        }

        cout << "\n";
    }
    cout << "Decrypted completely!!\n";

    // Write the messages and the final key in the output file
    ofstream fout("output.txt");

    for (int i=0;i<ciphertext_vector.size();i++) {
        fout << "m" << i+1 << " =  \"";
        for (int j=0;j<final_key.size();j++) {
            if (j >= ciphertext_vector[i].size()) {
                continue;
            }
            else {
                char ch = ciphertext_vector[i][j] ^ final_key[j];
                fout << ch;
            }
        }
        fout << "\"\n";
    }    

    fout << "\nKey = [ ";
    for(int i=0;i<final_key.size()-1;i++) {
        fout << (int)final_key[i] << ", ";
    }
    fout << (int)final_key[final_key.size()-1] << " ]\n";

    fout.close();

    cout << "\nWritten the messages and the key in output.txt\n";

    return 0;
}