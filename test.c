#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define ARRAY_LENGTH 4

unsigned int APHash(const int input){
    
   unsigned int hash = 3777171;
   unsigned int i = 7;
   hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (input) * (hash >> 3)) :
                               (~((hash << 11) + ((input) ^ (hash >> 5))));
   return hash % 1048576;
}


int main() {
    
    int message[ARRAY_LENGTH] = {1, 1, 1, 0};
    
    int output[ARRAY_LENGTH] = {0};
    int ans = APHash(45);
    printf("ans: %d\n", ans);
    /*printf("output: ");
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");*/

    return 0;
}