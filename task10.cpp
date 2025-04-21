#include <iostream>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

// Helper function to convert a string to hex
string toHex(const string& input) {
    stringstream ss;
    for (unsigned char c : input) {
        ss << hex << setw(2) << setfill('0') << (int)c;
    }
    return ss.str();
}

// Encrypt password 25 times using DES and a 16-bit salt
string encryptPassword(const string& password, const string& salt) {
    DES_cblock key;
    DES_key_schedule schedule;

    // Use salt to derive key (first 8 bytes, padded if needed)
    string paddedSalt = salt;
    paddedSalt.resize(8, '0');
    memcpy(key, paddedSalt.c_str(), 8);
    DES_set_key_unchecked(&key, &schedule);

    // Prepare input, pad to 8 bytes
    string data = password;
    data.resize(8, ' '); // simple padding

    DES_cblock input, output;
    memcpy(input, data.c_str(), 8);

    // Encrypt 25 times
    for (int i = 0; i < 25; ++i) {
        DES_ecb_encrypt(&input, &output, &schedule, DES_ENCRYPT);
        memcpy(input, output, 8);
    }

    return toHex(string((char*)output, 8));
}

// Generate random 2-character salt (16-bit)
string generateSalt() {
    string salt;
    for (int i = 0; i < 2; ++i) {
        salt += 'A' + rand() % 26;
    }
    return salt;
}

int main() {
    srand(time(nullptr));
    vector<string> encryptedList;

    cout << "🔐 Generating 10 Encrypted Passwords using DES and 16-bit salt:\n\n";

    for (int i = 0; i < 10; ++i) {
        string password = "Pass" + to_string(i + 1);
        string salt = generateSalt();
        string encrypted = encryptPassword(password, salt);

        cout << "Plain: " << password
             << " | Salt: " << salt
             << " | Encrypted: " << encrypted << endl;

        encryptedList.push_back(encrypted);
    }

    // Password verification
    cout << "\n🔎 Check a password\n";
    cout << "Enter password: ";
    string inputPassword;
    cin >> inputPassword;

    cout << "Enter salt used (2 letters): ";
    string inputSalt;
    cin >> inputSalt;

    string encryptedInput = encryptPassword(inputPassword, inputSalt);
    cout << "Encrypted input: " << encryptedInput << endl;

    bool found = false;
    for (const string& stored : encryptedList) {
        if (stored == encryptedInput) {
            found = true;
            break;
        }
    }

    if (found)
        cout << "✅ Password Match Found!\n";
    else
        cout << "❌ No Match Found.\n";

    return 0;
}
