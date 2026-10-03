#include <bits/stdc++.h>

using namespace std;

// Convert one byte to an 8-character binary string
string to_binary(unsigned char value) {
    string bits(8, '0');

    for (int i = 7; i >= 0; --i) {
        if (value % 2 == 1)
            bits[i] = '1';
        value /= 2;
    }

    return bits;
}

// XOR two binary strings
string manual_xor(const string& left, const string& right) {
    string result;

    for (size_t i = 0; i < left.size(); ++i) {
        if (left[i] == '0' && right[i] == '0')
            result += '0';
        else if (left[i] == '0' && right[i] == '1')
            result += '1';
        else if (left[i] == '1' && right[i] == '0')
            result += '1';
        else
            result += '0';
    }

    return result;
}

// Convert an 8-bit binary string back to its byte value.
unsigned char from_binary(const string& bits) {
    int value = 0;
    for (char bit : bits)
        value = value * 2 + (bit == '1' ? 1 : 0);
    return static_cast<unsigned char>(value);
}

string printable(unsigned char c) {
    if (c < 32 || c == 127)
        return "non-printable";
    return string(1, static_cast<char>(c));
}

// Print one character's mapping: a XOR b = result.
unsigned char show_xor(const string& a_name, unsigned char a,
                       const string& b_name, unsigned char b) {
    const string a_bits = to_binary(a);
    const string b_bits = to_binary(b);
    const string r_bits = manual_xor(a_bits, b_bits);
    const unsigned char result = from_binary(r_bits);

    cout << a_name << ": " << a_bits << "  (" << printable(a) << ")\n";
    cout << b_name << ": " << b_bits << "  (" << printable(b) << ")\n";
    cout << "XOR   : " << r_bits << "  (" << printable(result) << ")\n\n";
    return result;
}

int main() {
    string plaintext;
    string key;

    cout << "Plaintext: ";
    getline(cin, plaintext);
    cout << "Key (same number of characters as plaintext): ";
    getline(cin, key);

    if (plaintext.size() != key.size()) {
        cerr << "Error: plaintext and key must have the same length.\n";
        return 1;
    }

    // Encryption: C_i = P_i XOR K_i
    cout << "\n--- Encryption ---\n";
    vector<unsigned char> cipher;
    for (size_t i = 0; i < plaintext.size(); ++i) {
        cout << "Character " << i + 1 << "\n";
        cipher.push_back(show_xor("Plain ", static_cast<unsigned char>(plaintext[i]),
                                  "Key   ", static_cast<unsigned char>(key[i])));
    }

    // Decryption: P_i = C_i XOR K_i
    cout << "--- Decryption ---\n";
    string recovered;
    for (size_t i = 0; i < cipher.size(); ++i) {
        cout << "Character " << i + 1 << "\n";
        recovered += static_cast<char>(show_xor("Cipher", cipher[i],
                                                "Key   ", static_cast<unsigned char>(key[i])));
    }

    cout << "Recovered message: " << recovered << "\n";
    return 0;
}
