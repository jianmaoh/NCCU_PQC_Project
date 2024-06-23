#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define ARRAY_LENGTH 1



void hash_based_encryption(int *input, int *timeOfOWF, int *output, int length) {

    for (int i = 0; i < length; i++) {
        unsigned char data[SHA256_DIGEST_LENGTH];
        memcpy(data, &input[i], sizeof(int));
        printf("data after cpy: ");
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
        for (int j = 0; j < timeOfOWF[i]; j++) {
            unsigned char hash[SHA_DIGEST_LENGTH];
            SHA256(data, sizeof(int), hash);
            memcpy(data, hash, SHA256_DIGEST_LENGTH);
        }
        output[i] = data[0];  
    }
}

int main() {
    int input[ARRAY_LENGTH] = {31};
    int timeOfOWF[ARRAY_LENGTH] = {6};
    int output[ARRAY_LENGTH];

    int timeOfOWF2[ARRAY_LENGTH] = {4};
    int output2[ARRAY_LENGTH];

    int timeOfOWF3[ARRAY_LENGTH] = {2};
    int output3[ARRAY_LENGTH];

    hash_based_encryption(input, timeOfOWF, output, ARRAY_LENGTH);
    hash_based_encryption(input, timeOfOWF2, output2, ARRAY_LENGTH);
    hash_based_encryption(output2, timeOfOWF3, output3, ARRAY_LENGTH);

    printf("output: ");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");

    printf("output3: ");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        printf("%d ", output3[i]);
    }
    printf("\n");

    return 0;
}