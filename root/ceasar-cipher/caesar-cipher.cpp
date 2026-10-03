#include<bits/stdc++.h>
using namespace std;

int norm(int x, int m){
    return (x%m + m)%m;
}
//C = (P + s) mod 26, C = (P + s) mod 10 
string encrypt(string text, int s){
    string res="";

    for(auto c: text){
        if('A' <= c && c <= 'Z')
            res += (c - 'A' + s) % 26 + 'A';
        else if('a' <= c && c <= 'z')
            res += (c - 'a' + s) % 26 + 'a';  
        else if('0' <= c && c <= '9')  
            res += (c - '0' + s) % 10 + '0';
        else
            res += c;
    }

    return res;
}

// P = (C - s) mod 26 , P = (C - s) mod 10 
string decrypt(string text, int s){
    string res="";

    for(auto c: text){
        if('A' <= c && c <= 'Z')
            res += norm(c - 'A' - s, 26) + 'A';
        else if('a' <= c && c <= 'z')
            res += norm(c - 'a' - s, 26) + 'a';  
        else if('0' <= c && c <= '9')  
            res += norm(c - '0' - s, 10) + '0';
        else
            res += c;
    }

    return res;
}

//Key space size = lcm(26, 10) = 130
void key_gen(const string& c){
    int lcm = 26 * 10 / __gcd(26, 10);
    for(int key = 0; key < lcm; key++){
        cout << "Key: " << key << " | decrypted text: " << decrypt(c, key) << endl;
    }
}

int main(){
    string c = "khoor5672";
    cout << "Ciphertext: " << c << "\n--- Brute Force ---\n";
    key_gen(c);
    return 0;
}