#include <bits/stdc++.h>
using namespace std;

int norm(int x, int m) {
    return (x % m + m) % m;
}

string decrypt(const string& text, int s) {
    string res;

    for (char c : text) {
        if ('A' <= c && c <= 'Z')
            res += norm(c - 'A' - s, 26) + 'A';
        else if ('a' <= c && c <= 'z')
            res += norm(c - 'a' - s, 26) + 'a';
        else if ('0' <= c && c <= '9')
            res += norm(c - '0' - s, 10) + '0';
        else
            res += c;
    }

    return res;
}

int main() {
    const string ciphertext =
        "Aol xbpjr iyvdu mve qbtwz vcly aol shgf kvn iljhbzl zljbypaf "
        "vm h jpwoly zovbsk ulcly klwluk vu rllwpun aol hsnvypaot zljyla 9791";

    // Count how often each letter appears.
    int count[26] = {0};
    for (char c : ciphertext) {
        if ('a' <= c && c <= 'z')
            count[c - 'a']++;
        else if ('A' <= c && c <= 'Z')
            count[c - 'A']++;
    }

    cout << "Ciphertext: " << ciphertext << "\n\n";

    int peak = 0;
    for (int i = 0; i < 26; ++i) {
        cout << static_cast<char>('a' + i) << " : " << count[i] << "\n";
        if (count[i] > count[peak])
            peak = i;
    }

    // The most frequent letter in English text is 'e' (index 4: 'e' - 'a' = 4).
    // Formula: key = (peak - ('e' - 'a')) mod 26
    const int key = norm(peak - ('e' - 'a'), 26);

    cout << "\nMost frequent letter: " << static_cast<char>('a' + peak)
         << " (" << count[peak] << " times)\n";
    cout << "Assuming it is 'e': key = " << peak << " - " << ('e' - 'a') << " = " << key << "\n\n";
    // Decrypt: P = (C - key) mod 26
    cout << "Decrypted: " << decrypt(ciphertext, key) << "\n";
    return 0;
}
