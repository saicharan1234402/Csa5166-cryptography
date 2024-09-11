#include <stdio.h>
#include <math.h>
unsigned long long modExp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}
int main() {
    unsigned long long q = 23;
    unsigned long long x_A = 6;
    unsigned long long A = modExp(a, x_A, q);
    unsigned long long x_B = 15;
    unsigned long long B = modExp(a, x_B, q);
    unsigned long long S_A = modExp(B, x_A, q);
    unsigned long long S_B = modExp(A, x_B, q);
    printf("Alice's computed shared secret: %llu\n", S_A);
    printf("Bob's computed shared secret: %llu\n", S_B);

    if (S_A == S_B) {
        printf("Shared secret is established successfully!\n");
    } else {
        printf("Error in establishing shared secret.\n");
    }
    return 0;
}

