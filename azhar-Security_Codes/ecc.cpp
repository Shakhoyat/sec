#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Elliptic Curve: y^2 = x^3 + ax + b (mod p)
// Given parameters for G(3, 4): p = 17, a = 2, b = 0
const int64 p = 17;
const int64 a = 2;
const int64 b = 0;

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
    x = y1; y = x1 - (a / b) * y1;
    return g;
}

int64 modinv(int64 a, int64 m) {
    int64 x, y; int64 g = egcd(a, m, x, y);
    if (g != 1) return -1;
    x %= m; if (x < 0) x += m;
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

void print_point(string label, Point P) {
    if (P.isInf) cout << label << ": Point at Infinity (O)\n";
    else cout << label << ": (" << P.x << ", " << P.y << ")\n";
}

int main() {
    Point G(3, 4); // Given generator point

    cout << "Elliptic Curve: y^2 = x^3 + " << a << "x + " << b << " (mod " << p << ")\n";
    print_point("Given G", G);
    cout << "\n-----------------------------------------\n";

    // Method: Using Doubling & Addition (as shown in notes)
    // 2G = G + G (doubling)
    Point G2 = point_doubling(G);
    print_point("2G = G + G       (Doubling)", G2);

    // 4G = 2G + 2G (doubling)
    Point G4 = point_doubling(G2);
    print_point("4G = 2G + 2G     (Doubling)", G4);

    // 8G = 4G + 4G (doubling)
    Point G8 = point_doubling(G4);
    print_point("8G = 4G + 4G     (Doubling)", G8);

    // 9G = 8G + G (addition)
    Point G9 = point_addition(G8, G);
    print_point("9G = 8G + G      (Addition)", G9);

    cout << "-----------------------------------------\n";
    cout << "Final Answer for 9G: (" << G9.x << ", " << G9.y << ")\n";

    return 0;
}
