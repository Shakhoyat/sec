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

    ll m1 = 25, m2 = 23, k1 = 7, k2 = 9;

    // Encrypt m1: r11 = g^k1, r12 = m1 * gx^k1
    ll r11 = modpow(g, k1, p);
    ll r12 = (m1 * modpow(gx, k1, p)) % p;

    // Encrypt m2: r21 = g^k2, r22 = m2 * gx^k2
    ll r21 = modpow(g, k2, p);
    ll r22 = (m2 * modpow(gx, k2, p)) % p;

    cout << "original m1, m2: " << m1 << " " << m2 << "\n";

    // Homomorphic Multiplication:
    // r1_prod = (r11 * r21) mod p
    // r2_prod = (r12 * r22) mod p
    ll r1_prod = (r11 * r21) % p;
    ll r2_prod = (r12 * r22) % p;

    // Decryption:
    // (m1 * m2) = (r2_prod * (r1_prod^x)^(-1)) mod p
    ll dec = (r2_prod * modinv(modpow(r1_prod, x, p), p)) % p;

    cout << "decrypted (r1_prod, r2_prod): " << dec << "\n";
    cout << "m1 * m2: " << (m1 * m2) % p << "\n";

    return 0;
}