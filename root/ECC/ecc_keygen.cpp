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

int64 mod_pow(int64 a, int64 e, int64 m) {
    int64 r = 1; a %= m;
    while (e) {
        if (e & 1) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        e >>= 1;
    }
    return r;
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

bool validate_public_key(Point Q) {
    // Corner Case: Point at Infinity or point not satisfying y^2 = x^3 + ax + b (mod p)
    if (Q.isInf) return false;
    int64 left = mod_pow(Q.y, 2, p);
    int64 right = (mod_pow(Q.x, 3, p) + mul_mod(a, Q.x, p) + b) % p;
    return left == right;
}

void print_point(string label, Point P) {
    if (P.isInf) cout << label << ": Point at Infinity (O)\n";
    else cout << label << ": (" << P.x << ", " << P.y << ")\n";
}

int main() {
    Point G(3, 1); // Base point / Generator

    // Key Generation: Q = d * G (d: private key, Q: public key)
    // Alice Key Generation
    int64 d_A = 7;
    Point Q_A = scalar_mul(d_A, G);
    print_point("Alice Public Key (Q_A)", Q_A);
    cout << "Alice Public Key Valid? : " << (validate_public_key(Q_A) ? "YES" : "NO") << "\n";

    // Bob Key Generation
    int64 d_B = 5;
    Point Q_B = scalar_mul(d_B, G);
    print_point("Bob Public Key (Q_B)  ", Q_B);
    cout << "Bob Public Key Valid?   : " << (validate_public_key(Q_B) ? "YES" : "NO") << "\n";

    // ECDH Key Agreement: K = d_A * Q_B = d_B * Q_A = d_A * d_B * G
    Point K_Alice = scalar_mul(d_A, Q_B);
    Point K_Bob   = scalar_mul(d_B, Q_A);

    print_point("Shared Key (Alice)", K_Alice);
    print_point("Shared Key (Bob)  ", K_Bob);
    cout << "Match: " << (K_Alice == K_Bob ? "SUCCESS" : "FAILED") << "\n";

    return 0;
}
