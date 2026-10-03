#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Small prime modulus so the naive generator search completes instantly
const ll p = 10007;

mt19937_64 rng(random_device{}());

ll rand_range(ll lo, ll hi) {
    return lo + rng() % (hi - lo + 1);
}

ll mod_pow(ll a, ll e, ll m) {
    ll r = 1; a %= m;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % m;
        a = (__int128)a * a % m;
        e >>= 1;
    }
    return r;
}

ll egcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1; 
    ll g = egcd(b, a % b, x1, y1);
    x = y1; 
    y = x1 - (a / b) * y1;
    return g;
}

// Modular inverse via Extended Euclidean Algorithm
// (Used for operations mod p - 1, where p - 1 is composite and Fermat does not apply)
ll mod_inv(ll a, ll m) {
    ll x, y;
    ll g = egcd(a, m, x, y);
    if (g != 1) return -1;
    x %= m;
    if (x < 0) x += m;
    return x;
}

// ---------------- Naive Brute-Force Generator Finding ----------------
ll find_g_naive(ll p) {
    for (ll g = 2; g < p; g++) {
        ll cur = g, order = 1;
        while (cur != 1) { 
            cur = (__int128)cur * g % p; 
            order++; 
        }
        if (order == p - 1) return g;
    }
    return -1;
}

// ---------------- Key Generation (Alice / Signer) ----------------
// x is the private signing key, gx is the public verification key
void generate_keys(ll g, ll &x, ll &gx) {
    x  = rand_range(2, p - 2);
    gx = mod_pow(g, x, p);
}

// ---------------- Signing Routine ----------------
// Signs message m with private key x.
// Returns signature pair (s1, s2).
pair<ll, ll> sign_message(ll m, ll g, ll x) {
    ll k;
    // Ephemeral key k MUST be coprime to (p - 1) to have a modular inverse
    do {
        k = rand_range(2, p - 2);
    } while (__gcd(k, p - 1) != 1);

    ll s1 = mod_pow(g, k, p);
    ll k_inv = mod_inv(k, p - 1);

    // s2 = k^(-1) * (m - x * s1) mod (p - 1)
    ll x_s1 = (__int128)(x % (p - 1)) * (s1 % (p - 1)) % (p - 1);
    ll diff = (m % (p - 1) - x_s1) % (p - 1);
    if (diff < 0) diff += (p - 1);

    ll s2 = (__int128)k_inv * diff % (p - 1);

    return {s1, s2};
}

// ---------------- Verification Routine ----------------
// Verifies signature (s1, s2) using public key (p, g, gx)
bool verify_signature(ll m, pair<ll, ll> sig, ll g, ll gx) {
    ll s1 = sig.first;
    ll s2 = sig.second;

    // Check validity boundaries
    if (s1 <= 0 || s1 >= p || s2 < 0 || s2 >= (p - 1)) {
        return false;
    }

    // Left side  : g^m mod p
    ll left = mod_pow(g, m, p);

    // Right side : (gx^s1 * s1^s2) mod p
    ll term1 = mod_pow(gx, s1, p);
    ll term2 = mod_pow(s1, s2, p);
    ll right = (__int128)term1 * term2 % p;

    return left == right;
}

int main() {
    ll g = find_g_naive(p);
    if (g == -1) {
        cerr << "No generator found!\n";
        return 1;
    }

    ll x, gx;
    generate_keys(g, x, gx);

    printf("Domain parameters : p = %lld, g = %lld\n", p, g);
    printf("Private key (x)   : %lld\n", x);
    printf("Public key  (gx)  : %lld\n", gx);
    printf("Full Public Key   : (p = %lld, g = %lld, gx = %lld)\n\n", p, g, gx);

    // 1. Sign a message
    ll msg = 1234;
    pair<ll, ll> sig = sign_message(msg, g, x);
    printf("Message to Sign       : %lld\n", msg);
    printf("Generated Signature   : (s1 = %lld, s2 = %lld)\n", sig.first, sig.second);

    // 2. Verify with valid message
    bool valid = verify_signature(msg, sig, g, gx);
    printf("Verification (Valid)  : %s\n\n", valid ? "SIGNATURE VALID" : "SIGNATURE INVALID");

    // 3. Test tampering (Message changed to msg + 1)
    ll tampered_msg = msg + 1;
    bool tampered_valid = verify_signature(tampered_msg, sig, g, gx);
    printf("Tampered Message      : %lld\n", tampered_msg);
    printf("Verification (Tamper) : %s\n", tampered_valid ? "SIGNATURE VALID" : "SIGNATURE INVALID");

    return 0;
}