#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Elliptic Curve Parameters: y^2 = x^3 + ax + b (mod p)
// Prime field p = 17, a = 2, b = 2
// Generator point G = (3, 1), Order of G is prime n = 19
const int64 p = 17;
const int64 a = 2;
const int64 b = 2;
const int64 n = 19; // Order of generator base point G

int64 mul_mod(int64 a, int64 b, int64 m) {
    return (int64)((__int128)a * b % m);
}

int64 egcd(int64 a, int64 b, int64 &x, int64 &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int64 x1, y1;
    int64 g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int64 modinv(int64 a, int64 m) {
    int64 x, y;
    int64 g = egcd(a, m, x, y);
    if (g != 1) return -1;
    x %= m;
    if (x < 0) x += m;
    return x;
}

struct Point {
    int64 x, y;
    bool isInf;
    Point(int64 x = 0, int64 y = 0, bool isInf = false) : x(x), y(y), isInf(isInf) {}
};

// -------------------------------------------------------------
// Point Doubling: P + P = 2P
// s = (3*x1^2 + a) / (2*y1) (mod p)
// x3 = s^2 - 2*x1 (mod p)
// y3 = s*(x1 - x3) - y1 (mod p)
// -------------------------------------------------------------
Point point_doubling(Point P) {
    if (P.isInf || P.y == 0) return Point(0, 0, true);

    int64 x1 = P.x, y1 = P.y;

    int64 num = (3 * mul_mod(x1, x1, p) + a) % p;
    int64 den = modinv((2 * y1) % p, p);
    int64 s = mul_mod(num, den, p);

    int64 x3 = ((mul_mod(s, s, p) - 2 * x1) % p + p) % p;
    int64 y3 = ((mul_mod(s, ((x1 - x3) % p + p) % p, p) - y1) % p + p) % p;

    return Point(x3, y3);
}

// -------------------------------------------------------------
// Point Addition: P + Q
// s = (y2 - y1) / (x2 - x1) (mod p)
// x3 = s^2 - x1 - x2 (mod p)
// y3 = s*(x1 - x3) - y1 (mod p)
// -------------------------------------------------------------
Point point_addition(Point P, Point Q) {
    if (P.isInf) return Q;
    if (Q.isInf) return P;

    int64 x1 = P.x, y1 = P.y;
    int64 x2 = Q.x, y2 = Q.y;

    // P + (-P) = Point at Infinity
    if (x1 == x2 && (y1 + y2) % p == 0) return Point(0, 0, true);

    // If P == Q, use Point Doubling
    if (x1 == x2 && y1 == y2) return point_doubling(P);

    int64 num = ((y2 - y1) % p + p) % p;
    int64 den = modinv(((x2 - x1) % p + p) % p, p);
    int64 s = mul_mod(num, den, p);

    int64 x3 = ((mul_mod(s, s, p) - x1 - x2) % p + p) % p;
    int64 y3 = ((mul_mod(s, ((x1 - x3) % p + p) % p, p) - y1) % p + p) % p;

    return Point(x3, y3);
}

// Scalar multiplication: k * P (Double-and-Add)
Point scalar_mul(int64 k, Point P) {
    Point res(0, 0, true);
    while (k > 0) {
        if (k & 1) res = point_addition(res, P);
        P = point_doubling(P);
        k >>= 1;
    }
    return res;
}

struct Signature {
    int64 r, s;
};

// -------------------------------------------------------------
// ECDSA Signing:
// 1. Pick ephemeral key k in [1, n-1] with gcd(k, n) = 1
// 2. Compute (x1, y1) = k * G
// 3. r = x1 mod n
// 4. s = k^(-1) * (m + d * r) mod n
// -------------------------------------------------------------
Signature sign_message(int64 m, int64 d, Point G, int64 k) {
    Point kG = scalar_mul(k, G);
    int64 r = kG.x % n;
    if (r == 0) return {0, 0};

    int64 k_inv = modinv(k, n);
    int64 s = mul_mod(k_inv, (m + mul_mod(d, r, n)) % n, n);

    return {r, s};
}

// -------------------------------------------------------------
// ECDSA Verification:
// 1. w = s^(-1) mod n
// 2. u1 = (m * w) mod n,  u2 = (r * w) mod n
// 3. Point P = u1 * G + u2 * Q
// 4. Valid if P.x mod n == r
// -------------------------------------------------------------
bool verify_signature(int64 m, Signature sig, Point Q, Point G) {
    if (sig.r <= 0 || sig.r >= n || sig.s <= 0 || sig.s >= n)
        return false;

    int64 w = modinv(sig.s, n);
    int64 u1 = mul_mod(m, w, n);
    int64 u2 = mul_mod(sig.r, w, n);

    Point term1 = scalar_mul(u1, G);
    Point term2 = scalar_mul(u2, Q);
    Point P = point_addition(term1, term2);

    if (P.isInf) return false;
    return (P.x % n) == sig.r;
}

void print_point(string label, Point P) {
    if (P.isInf) cout << label << ": Point at Infinity (O)\n";
    else cout << label << ": (" << P.x << ", " << P.y << ")\n";
}

int main() {
    Point G(3, 1); // Generator base point

    cout << "--- Elliptic Curve Digital Signature Algorithm (ECDSA) ---\n";
    cout << "Curve: y^2 = x^3 + " << a << "x + " << b << " (mod " << p << ")\n";
    print_point("Base Point G", G);
    cout << "Order of G (n)    : " << n << "\n\n";

    // 1. Key Generation
    int64 private_key = 7; // Signer's private key d in [1, n-1]
    Point public_key = scalar_mul(private_key, G); // Q = d * G
    cout << "Private Key (d)   : " << private_key << "\n";
    print_point("Public Key (Q)    ", public_key);
    cout << "\n";

    // 2. Signing a message
    int64 message = 12; // Message hash / integer
    int64 k = 5;        // Ephemeral random scalar with gcd(k, n) = 1
    Signature sig = sign_message(message, private_key, G, k);

    cout << "Message (m)       : " << message << "\n";
    cout << "Signature (r, s)  : (" << sig.r << ", " << sig.s << ")\n\n";

    // 3. Verification of legitimate signature
    bool valid = verify_signature(message, sig, public_key, G);
    cout << "Verification (Original Message) : " << (valid ? "ACCEPTED (Signature is VALID)" : "REJECTED") << "\n";

    // 4. Verification of tampered message
    int64 tampered_message = 15;
    bool tampered_valid = verify_signature(tampered_message, sig, public_key, G);
    cout << "Verification (Tampered Message) : " << (tampered_valid ? "ACCEPTED" : "REJECTED (Tampering Detected)") << "\n";

    return 0;
}
