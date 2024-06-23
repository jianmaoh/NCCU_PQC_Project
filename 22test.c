#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define ARRAY_LENGTH 3



void hash_based_encryption(int *aaa, int *time, int *ot, int length) {

    for (int i = 0; i < length; i++) {

        unsigned char data[SHA256_DIGEST_LENGTH];
        memset(data, 0, sizeof(data));
        data[i] = aaa[i];

        for (int j = 0; j < time[i]; j++) {
            unsigned char hash[SHA_DIGEST_LENGTH];
            SHA256(data, sizeof(int), hash);
            data[i] = hash[i];
        }
        ot[i] = data[i];
        memset(data, 0, sizeof(data));
    }
}

int main() {
    int input[32] = {31,45,27};
    //input[0] = 31;
    int timeOfOWF[ARRAY_LENGTH] = {3,3,3};
    int output[8];
    int input2[32] = {31,45,27};
    //input2[0] = 31;
    int timeOfOWF2[ARRAY_LENGTH] = {3,1,0};
    int output2[8];

    int timeOfOWF3[ARRAY_LENGTH] = {0,2,3};
    int output3[8];

    hash_based_encryption(input, timeOfOWF, output, ARRAY_LENGTH);
    hash_based_encryption(input2, timeOfOWF2, output2, ARRAY_LENGTH);
    hash_based_encryption(output2, timeOfOWF3, output3, ARRAY_LENGTH);
    for(int i = 0 ; i < ARRAY_LENGTH ; i++){
        printf("output is : %d\n",output[i]);
        printf("output3 is : %d\n",output3[i]);
        if(output[i] != output3[i]){
            printf("False\n");
        }else{
            printf("True\n");
        }    
    }
    

    return 0;
}