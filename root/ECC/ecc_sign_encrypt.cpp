#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

const int64 p = 17;
const int64 a = 2;
const int64 b = 2;
const int64 n = 19;

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

    bool operator==(const Point &o) const {
        if (isInf && o.isInf) return true;
        if (isInf || o.isInf) return false;
        return x == o.x && y == o.y;
    }
};

Point point_doubling(Point P) {
    // Corner Case 1: Point at Infinity (P == O)
    // Corner Case 2: Vertical Tangent (y1 == 0 -> 2y1 = 0 mod p)
    if (P.isInf || P.y == 0) return Point(0, 0, true);

    int64 x1 = P.x, y1 = P.y;

    int64 num = (3 * mul_mod(x1, x1, p) + a) % p;
    int64 den = modinv((2 * y1) % p, p);
    int64 s = mul_mod(num, den, p);

    int64 x3 = ((mul_mod(s, s, p) - 2 * x1) % p + p) % p;
    int64 y3 = ((mul_mod(s, ((x1 - x3) % p + p) % p, p) - y1) % p + p) % p;

    return Point(x3, y3);
}

Point point_addition(Point P, Point Q) {
    // Corner Case 1: Identity Element (P + O = P, O + Q = Q)
    if (P.isInf) return Q;
    if (Q.isInf) return P;

    int64 x1 = P.x, y1 = P.y;
    int64 x2 = Q.x, y2 = Q.y;

    // Corner Case 2: Vertical Line / Inverse Points (P + (-P) = O)
    if (x1 == x2 && (y1 + y2) % p == 0) return Point(0, 0, true);

    // Corner Case 3: Identical Points (P == Q -> Doubling)
    if (x1 == x2 && y1 == y2) return point_doubling(P);

    int64 num = ((y2 - y1) % p + p) % p;
    int64 den = modinv(((x2 - x1) % p + p) % p, p);
    int64 s = mul_mod(num, den, p);

    int64 x3 = ((mul_mod(s, s, p) - x1 - x2) % p + p) % p;
    int64 y3 = ((mul_mod(s, ((x1 - x3) % p + p) % p, p) - y1) % p + p) % p;

    return Point(x3, y3);
}

Point scalar_mul(int64 k, Point P) {
    Point res(0, 0, true);
    while (k > 0) {
        if (k & 1) res = point_addition(res, P);
        P = point_doubling(P);
        k >>= 1;
    }
    return res;
}

Point point_neg(Point P) {
    // Corner Case: Negative of Point at Infinity is O
    if (P.isInf) return P;
    return Point(P.x, (p - P.y % p) % p);
}

struct Signature {
    int64 r, s;
};

struct Ciphertext {
    Point c1, c2;
};

// ECDSA Sign: r = (k*G).x mod n, s = k^(-1) * (m + d*r) mod n
Signature sign_message(int64 m, int64 d, Point G, int64 k) {
    Point kG = scalar_mul(k, G);
    int64 r = kG.x % n;
    // Corner Case: r == 0
    if (r == 0) return {0, 0};

    int64 k_inv = modinv(k, n);
    int64 s = mul_mod(k_inv, (m + mul_mod(d, r, n)) % n, n);
    // Corner Case: s == 0
    if (s == 0) return {0, 0};

    return {r, s};
}

// ECDSA Verify: w = s^(-1) mod n, P = (m*w)*G + (r*w)*Q, check P.x mod n == r
bool verify_signature(int64 m, Signature sig, Point Q, Point G) {
    // Corner Case: Signature range validity
    if (sig.r <= 0 || sig.r >= n || sig.s <= 0 || sig.s >= n) return false;

    int64 w = modinv(sig.s, n);
    int64 u1 = mul_mod(m, w, n);
    int64 u2 = mul_mod(sig.r, w, n);

    Point term1 = scalar_mul(u1, G);
    Point term2 = scalar_mul(u2, Q);
    Point P = point_addition(term1, term2);

    // Corner Case: Point at Infinity
    if (P.isInf) return false;
    return (P.x % n) == sig.r;
}

// ECC Encrypt: C1 = k * G, C2 = M + k * Y
Ciphertext encrypt(Point M, Point G, Point Y, int64 k) {
    Point C1 = scalar_mul(k, G);
    Point C2 = point_addition(M, scalar_mul(k, Y));
    return {C1, C2};
}

// ECC Decrypt: M = C2 - x * C1
Point decrypt(Ciphertext ct, int64 x) {
    Point shared = scalar_mul(x, ct.c1);
    return point_addition(ct.c2, point_neg(shared));
}

int main() {
    Point G(3, 1);

    // Alice (Signer)
    int64 d_A = 7;
    Point Q_A = scalar_mul(d_A, G);

    // Bob (Receiver)
    int64 x_B = 5;
    Point Y_B = scalar_mul(x_B, G);

    // Sign & Encrypt
    int64 message_val = 5;
    Point M(5, 1);

    Signature sig = sign_message(message_val, d_A, G, 5);
    Ciphertext ct = encrypt(M, G, Y_B, 3);

    // Decrypt & Verify
    Point decrypted_point = decrypt(ct, x_B);
    bool valid = verify_signature(decrypted_point.x, sig, Q_A, G);

    cout << "Decrypted Point: (" << decrypted_point.x << ", " << decrypted_point.y << ")\n";
    cout << "Signature Verification: " << (valid ? "VALID" : "INVALID") << "\n";

    return 0;
}
