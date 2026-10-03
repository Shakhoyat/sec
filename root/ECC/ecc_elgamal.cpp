#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

const int64 p = 17;
const int64 a = 2;
const int64 b = 2;

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
    // Point at Infinity (P == O)
    // Vertical Tangent (y1 == 0 -> 2y1 = 0 mod p)
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
    //Identity Element (P + O = P, O + Q = Q)
    if (P.isInf) return Q;
    if (Q.isInf) return P;

    int64 x1 = P.x, y1 = P.y;
    int64 x2 = Q.x, y2 = Q.y;

    //Inverse Points (P + (-P) = O)
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
    // Negative of Point at Infinity
    if (P.isInf) return P;
    return Point(P.x, (p - P.y % p) % p);
}

struct Ciphertext {
    Point c1, c2;
};

// C1 = k * G
// C2 = M + k * Y (where Y = x * G is public key)
Ciphertext encrypt(Point M, Point G, Point Y, int64 k) {
    Point C1 = scalar_mul(k, G);
    Point C2 = point_addition(M, scalar_mul(k, Y));
    return {C1, C2};
}

// M = C2 - x * C1 = C2 + (-x * C1)
Point decrypt(Ciphertext ct, int64 x) {
    Point shared = scalar_mul(x, ct.c1);
    return point_addition(ct.c2, point_neg(shared));
}

void print_point(string label, Point P) {
    if (P.isInf) cout << label << ": Point at Infinity (O)\n";
    else cout << label << ": (" << P.x << ", " << P.y << ")\n";
}

int main() {
    Point G(3, 1);
    Point M(5, 1);

    int64 private_key = 5;
    Point public_key = scalar_mul(private_key, G);

    int64 k = 7;
    Ciphertext ct = encrypt(M, G, public_key, k);
    print_point("Plaintext M ", M);
    print_point("Ciphertext C1", ct.c1);
    print_point("Ciphertext C2", ct.c2);

    Point decrypted = decrypt(ct, private_key);
    print_point("Decrypted M ", decrypted);
    cout << "Match: " << (decrypted == M ? "SUCCESS" : "FAILED") << "\n";

    return 0;
}
