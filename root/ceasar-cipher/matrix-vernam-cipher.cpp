#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<string> > Matrix;

// Convert one byte to an 8-character binary string without bitwise operators.
string to_binary(unsigned char value) {
    string bits(8, '0');

    for (int i = 7; i >= 0; --i) {
        if (value % 2 == 1)
            bits[i] = '1';
        value /= 2;
    }

    return bits;
}

// XOR two binary strings, using the four possible bit combinations.
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
    if (c == ' ')
        return "space";
    if (c < 32 || c == 127)
        return "non-printable";
    return string(1, static_cast<char>(c));
}

// Swap rows and columns.
Matrix transpose(const Matrix& m) {
    Matrix t(m[0].size(), vector<string>(m.size()));
    for (size_t r = 0; r < m.size(); ++r)
        for (size_t c = 0; c < m[0].size(); ++c)
            t[c][r] = m[r][c];
    return t;
}

void print_strings(const Matrix& m) {
    for (const vector<string>& row : m) {
        cout << "  ";
        for (const string& cell : row)
            cout << '"' << cell << "\"  ";
        cout << '\n';
    }
    cout << '\n';
}

void print_binary(const Matrix& m) {
    for (const vector<string>& row : m) {
        for (const string& cell : row) {
            cout << "  ";
            for (unsigned char c : cell)
                cout << to_binary(c) << ' ';
            cout << '\n';
        }
    }
    cout << '\n';
}

// XOR two same-shaped matrices element by element, byte by byte.
Matrix xor_matrices(const Matrix& a, const string& a_name,
                    const Matrix& b, const string& b_name) {
    Matrix result(a.size(), vector<string>(a[0].size()));

    for (size_t r = 0; r < a.size(); ++r) {
        for (size_t c = 0; c < a[0].size(); ++c) {
            cout << "Element (" << r + 1 << "," << c + 1 << ")\n";

            for (size_t i = 0; i < a[r][c].size(); ++i) {
                const unsigned char x = static_cast<unsigned char>(a[r][c][i]);
                const unsigned char y = static_cast<unsigned char>(b[r][c][i]);
                const string x_bits = to_binary(x);
                const string y_bits = to_binary(y);
                const string r_bits = manual_xor(x_bits, y_bits);
                result[r][c] += static_cast<char>(from_binary(r_bits));

                cout << "  " << a_name << ": " << x_bits << "  (" << printable(x) << ")\n";
                cout << "  " << b_name << ": " << y_bits << "  (" << printable(y) << ")\n";
                cout << "  XOR   : " << r_bits << "  ("
                     << printable(from_binary(r_bits)) << ")\n\n";
            }
        }
    }

    return result;
}

int main() {
    const Matrix message = {
        {"hell", "o ku"},
        {"et c", "yber"}
    };

    const Matrix key = {
        {"abcd", "efgh"},
        {"ijkl", "mnop"}
    };

    cout << "Message matrix:\n";
    print_strings(message);

    cout << "Key matrix:\n";
    print_strings(key);

    // Encryption Formula: C = (Key XOR Message)^T
    cout << "--- Encryption: (key XOR message), then transpose ---\n";
    const Matrix xored = xor_matrices(key, "Key    ", message, "Message");

    cout << "After XOR:\n";
    print_strings(xored);

    const Matrix cipher = transpose(xored);
    cout << "Ciphertext matrix after transpose (binary):\n";
    print_binary(cipher);

    // Decryption Formula: Decrypted = (Cipher^T) XOR Key
    cout << "--- Decryption: transpose back, then XOR with key ---\n";
    const Matrix back = transpose(cipher);
    const Matrix decrypted = xor_matrices(back, "Cipher ", key, "Key    ");

    cout << "Decrypted matrix:\n";
    print_strings(decrypted);
    return 0;
}
