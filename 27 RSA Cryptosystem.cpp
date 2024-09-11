#include <stdio.h>
#include <math.h>
int modExp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}
void decryptMessage(int *ciphertext, int length, int e, int n) {
    for (int i = 0; i < length; i++) {
        int decryptedValue = 0;
        for (int j = 0; j <= 25; j++) {
            if (modExp(j, e, n) == ciphertext[i]) {
                decryptedValue = j;
                break;
            }
        }
        printf("%c", decryptedValue + 'A');
    }
    printf("\n");
}
int main() {
    int e = 17;
    int n = 3233;
    int ciphertext[] = {2790, 2201, 851, 851, 3041};
    int length = sizeof(ciphertext) / sizeof(ciphertext[0]);
    printf("Decrypted message: ");
    decryptMessage(ciphertext, length, e, n);
    return 0;
}

