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

    ll m = 3, k = 6, gamma = 4;

    // Homomorphic Exponentiation:
    // r1_exp = (g^k)^gamma mod p
    // r2_exp = (m * gx^k)^gamma mod p
    ll r1 = modpow(modpow(g, k, p), gamma, p);
    ll r2 = modpow((m * modpow(gx, k, p) % p), gamma, p);

    ll mp = modpow(m, gamma, p); // Expected plaintext: m^gamma mod p

    // Decryption:
    // (m^gamma) = (r2_exp * (r1_exp^x)^(-1)) mod p
    ll dec = (r2 * modinv(modpow(r1, x, p), p)) % p;

    cout << "m ^ gamma : " << mp << endl;
    cout << "decrypted result: " << dec << endl;

    return 0;
}