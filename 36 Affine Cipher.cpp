#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int is_coprime(int a) {
    int n = 26;
    int temp;
    while (n != 0) {
        temp = a % n;
        a = n;
        n = temp;
    }
    return (a == 1);
}
int mod_inverse(int a) {
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) {
            return x;
        }
    }
    return -1;
}
void encrypt(const char* plaintext, char* ciphertext, int a, int b) {
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = ((a * (plaintext[i] - 'A') + b) % 26) + 'A';
        } else if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            ciphertext[i] = ((a * (plaintext[i] - 'a') + b) % 26) + 'a';
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[length] = '\0';
}
void decrypt(const char* ciphertext, char* plaintext, int a, int b) {
    int length = strlen(ciphertext);
    int a_inv = mod_inverse(a);
    if (a_inv == -1) {
        printf("Inverse of a does not exist, decryption is not possible.\n");
        exit(1);
    }
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = (a_inv * (ciphertext[i] - 'A' - b + 26) % 26) + 'A';
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = (a_inv * (ciphertext[i] - 'a' - b + 26) % 26) + 'a';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[length] = '\0';
}
int main() {
    int a, b;
    char plaintext[256], ciphertext[256], decryptedtext[256];
    printf("Enter values for a and b (with a coprime to 26): ");
    scanf("%d %d", &a, &b);
    if (!is_coprime(a)) {
        printf("The value of a is not coprime with 26. Please choose a different value.\n");
        return 1;
    }
    printf("Enter the plaintext: ");
    scanf(" %[^\n]s", plaintext);
    encrypt(plaintext, ciphertext, a, b);
    printf("Ciphertext: %s\n", ciphertext);
    decrypt(ciphertext, decryptedtext, a, b);
    printf("Decrypted text: %s\n", decryptedtext);
    return 0;
}

