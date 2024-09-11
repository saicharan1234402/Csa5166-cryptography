#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define P 23    // A small prime number for simplicity
#define Q 11    // A small prime number for simplicity
#define G 5     // A primitive root modulo P for simplicity
#define X 6     // Private key
#define H 9     // Hash of the message (in practice, this would be a hash of the message)

int mod_exp(int base, int exp, int mod) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

void dsa_sign(int hash, int *r, int *s) {
    int k = rand() % (Q - 1) + 1; // Random number k
    *r = mod_exp(G, k, P) % Q;
    *s = ((k * X + *r * hash) % Q) % Q;
}

int main() {
    srand(time(NULL));

    int hash = H;
    int r1, s1, r2, s2;

    dsa_sign(hash, &r1, &s1);
    dsa_sign(hash, &r2, &s2);

    printf("DSA Signature 1: (r: %d, s: %d)\n", r1, s1);
    printf("DSA Signature 2: (r: %d, s: %d)\n", r2, s2);

    return 0;
}

