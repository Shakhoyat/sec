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

    ll m = 34; // Message to sign

    // Choose ephemeral key k such that gcd(k, p-1) = 1
    ll k = 2;
    ll x3, y3;
    while(k < p-1){
        if(egcd(k, p-1, x3, y3) == 1)
            break;
        k++;
    }

    // Signature Generation:
    // r1 = g^k mod p
    // r2 = (k^(-1) * (m - x * r1)) mod (p-1)
    ll r1 = modpow(g, k, p);
    ll r2 = (modinv(k, p-1) * norm((m - x * r1), p-1)) % (p-1);

    // Verification:
    // LHS = (gx^r1 * r1^r2) mod p
    // RHS = g^m mod p
    ll lhs = (modpow(gx, r1, p) * modpow(r1, r2, p)) % p;
    ll rhs = modpow(g, m, p);

    cout << "Signature (r1, r2): " << r1 << " " << r2 << "\n";
    cout << "verification result --> lhs, rhs: " << lhs << " " << rhs << "\n";
    if(lhs == rhs)
        cout << "valid\n";
    else
        cout << "invalid\n";

    return 0;
}