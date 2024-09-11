#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_TEXT_LENGTH 100
void generate_random_key(int key[], int length) {
    for (int i = 0; i < length; i++) {
        key[i] = rand() % 26;
    }
}

void encrypt(const char *plaintext, char *ciphertext, int key[], int length) {
    for (int i = 0; i < length; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = ((plaintext[i] - 'A' + key[i]) % 26) + 'A';
        } else if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            ciphertext[i] = ((plaintext[i] - 'a' + key[i]) % 26) + 'a';
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[length] = '\0';
}
int main() {
    char plaintext[MAX_TEXT_LENGTH];
    char ciphertext[MAX_TEXT_LENGTH];
    int key[MAX_TEXT_LENGTH];
    int length;
    srand(time(NULL));
    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    length = strlen(plaintext);
        if (plaintext[length - 1] == '\n') {
        plaintext[length - 1] = '\0';
        length--;
    }
    generate_random_key(key, length);
    encrypt(plaintext, ciphertext, key, length);
    printf("Key: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", key[i]);
    }
    printf("\n");
    printf("Ciphertext: %s\n", ciphertext);
    return 0;
}

