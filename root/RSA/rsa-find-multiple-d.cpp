#include<bits/stdc++.h>
using namespace std;

using ll = __int128_t;

ostream& operator<<(ostream& out, ll x){
    if(x == 0) return out << 0;
    string s = "";
    while(x){
        ll r = x%10;
        s += (r + '0');
        x /= 10;
    }
    reverse(s.begin(), s.end());
    out << s;
    return out;
}

istream& operator>>(istream& in, ll &x){
    string s;
    in >> s;
    x = 0;
    for(char c : s){
        x = x * 10 + (c - '0');
    }
    return in;
}

ll norm(ll x, ll m){
    return (x%m + m)%m;
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
    ll p = 53, q = 61;

    ll n = p*q;
    ll phi = (p-1) * (q-1);

    ll e = 2;
    ll x,y;
    while(e < phi){
        if(egcd(e, phi, x, y) == 1)
            break;
        e++;
    }

    // Primary private key: d1 = e^(-1) mod phi
    ll d1 = modinv(e, phi);

    // Equivalent private keys: d_k = d1 + k * phi (since e * d_k = e*d1 + k*e*phi = 1 mod phi)
    ll d2 = d1 + phi;
    ll d3 = d1 + 2*phi;

    cout << "Computed d1, d2, d3: " << d1 << " " << d2 << " " << " " << d3 << "\n";
    cout << "Verification: "<< "\n";

    if(e*d1 % phi == 1)
        cout << "Valid d1\n";
    
    if(e*d2 % phi == 1)
        cout << "Valid d2\n";

    if(e*d3 % phi == 1)
        cout << "Valid d3\n";

    return 0;
}