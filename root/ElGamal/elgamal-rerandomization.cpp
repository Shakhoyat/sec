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

    ll m = 35, k1 = 6;

    // Initial Encryption: r1 = g^k1, r2 = m * gx^k1
    ll r1 = modpow(g, k1, p);
    ll r2 = (m * modpow(gx, k1, p)) % p;

    ll k2 = 7; // Re-randomization ephemeral key

    // Re-randomization:
    // r1_prime = (r1 * g^k2) mod p
    // r2_prime = (r2 * gx^k2) mod p
    ll r1_prime = (r1 * modpow(g, k2, p)) % p;
    ll r2_prime = (r2 * modpow(gx, k2, p)) % p;

    // Decryption:
    // m = (r2_prime * (r1_prime^x)^(-1)) mod p
    ll dec = (r2_prime * modinv(modpow(r1_prime, x, p), p)) % p;

    cout << "original: " << m << endl;
    cout << "recovered: " << dec << endl;

    return 0;
}