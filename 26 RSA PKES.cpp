#include <stdio.h>
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
int modInverse(int e, int phi) {
    int m0 = phi, t, q;
    int x0 = 0, x1 = 1;
    if (phi == 1)
        return 0;
    while (e > 1) {
        q = e / phi;
        t = phi;
        phi = e % phi;
        e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}
int main() {
    int n = 3599;
    int p = 59;
    int q = 61;
    int phi = (p - 1) * (q - 1);
    int new_e = 37;
    if (gcd(new_e, phi) != 1) {
        printf("The chosen e is not coprime with phi(n). Choose another e.\n");
        return 1;
    }
    int new_d = modInverse(new_e, phi);
    printf("New public key (e, n): (%d, %d)\n", new_e, n);
    printf("New private key (d, n): (%d, %d)\n", new_d, n);
    return 0;
}

