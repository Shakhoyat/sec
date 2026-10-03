#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll norm(ll x, ll m){
    return (x%m + m)%m;
}

ll modpow(ll a, ll e, ll m){
    ll r = 1;
    a %= m;

    while(e){
        if(e & 1)
            r = (r*a) % m;
        a = (a * a) % m;
        e >>= 1;
    }
    return r;
}

ll egcd(ll a, ll b, ll &x, ll &y){
    if(b == 0){
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll g = egcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;
    return g;
}

ll modinv(ll a, ll m){
    ll x, y;
    ll g = egcd(a, m, x, y);
    return norm(x, m);
}

int main(){
    // Key Generation:
    // p = prime modulus, g = generator (alpha)
    // x = private key, gx = g^x mod p = public key (beta)
    ll p = 79, g = 6, x = 5;
    ll gx = modpow(g, x, p);

    ll m = 25, k = 7; // m = plaintext, k = ephemeral key

    cout << "original: " << m << "\n";

    // Encryption:
    // r1 = g^k mod p
    // r2 = (m * gx^k) mod p
    ll r1 = modpow(g, k, p);
    ll r2 = (m * modpow(gx, k, p)) % p;

    cout << "ciphertext (r1, r2): " << r1 << " " << r2 << "\n";

    // Decryption:
    // m = (r2 * (r1^x)^(-1)) mod p
    ll dec = (r2 * modinv(modpow(r1, x, p), p)) % p;

    cout << "recovered: " << dec << "\n";
    return 0;
}