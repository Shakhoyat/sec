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

struct Ciphertext {
    Point c1, c2;
};

Ciphertext encrypt(Point M, Point G, Point Y, int64 k) {
    Point C1 = scalar_mul(k, G);
    Point C2 = point_addition(M, scalar_mul(k, Y));
    return {C1, C2};
}

Point decrypt(Ciphertext ct, int64 x) {
    Point shared = scalar_mul(x, ct.c1);
    return point_addition(ct.c2, point_neg(shared));
}

// Re-randomization Formulas:
// C1_new = C1 + k_prime * G
// C2_new = C2 + k_prime * Y
Ciphertext rerandomize(Ciphertext ct, Point G, Point Y, int64 k_prime) {
    Point c1_new = point_addition(ct.c1, scalar_mul(k_prime, G));
    Point c2_new = point_addition(ct.c2, scalar_mul(k_prime, Y));
    return {c1_new, c2_new};
}

void print_point(string label, Point P) {
    if (P.isInf) cout << label << ": Point at Infinity (O)\n";
    else cout << label << ": (" << P.x << ", " << P.y << ")\n";
}

int main() {
    Point G(3, 1);
    int64 private_key = 5;
    Point public_key = scalar_mul(private_key, G);

    Point M(5, 1);
    Ciphertext ct_orig = encrypt(M, G, public_key, 7);

    // Re-randomize
    Ciphertext ct_refreshed = rerandomize(ct_orig, G, public_key, 3);
    print_point("Original C1 ", ct_orig.c1);
    print_point("Refreshed C1", ct_refreshed.c1);

    Point dec_orig = decrypt(ct_orig, private_key);
    Point dec_refreshed = decrypt(ct_refreshed, private_key);

    print_point("Decrypted Original ", dec_orig);
    print_point("Decrypted Refreshed", dec_refreshed);
    cout << "Match: " << (dec_orig == M && dec_refreshed == M ? "SUCCESS" : "FAILED") << "\n";

    return 0;
}
