#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

#define ARRAY_LENGTH 10
#define LIMIT 256



void hash_based_encryption(int *aaa, int *time, int *ot, int length) {

    for (int i = 0; i < length; i++) {
        unsigned char data[SHA256_DIGEST_LENGTH];
        memset(data, 0, sizeof(data));
        //memcpy(data, &aaa[i * SHA256_DIGEST_LENGTH / sizeof(int)], SHA256_DIGEST_LENGTH);
        data[i] = aaa[i];
        printf("data after cpy: ");
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%d ", data[i]);
        }
        
        printf("\n");
        for (int j = 0; j < time[i]; j++) {
            unsigned char hash[SHA_DIGEST_LENGTH];
            SHA256(data,SHA256_DIGEST_LENGTH , hash);
            printf("第 %d 次: \n",j+1);
            for(int i=0;i<SHA_DIGEST_LENGTH;i++){
                printf("%d ",hash[i]);
            }
            printf("\n");
            //memcpy(data, hash, SHA256_DIGEST_LENGTH);
            data[i] = hash[i];
        }
        //memcpy(ot, data, sizeof(data));
        ot[i] = data[i];
        memset(data, 0, sizeof(data));
        //memset(hash, 0, sizeof(hash));
    }
}

int main() {
    int input[10] = {};
    //input[0] = 31;
    int timeOfOWF[ARRAY_LENGTH] = {10,7,5,11,10,8,9,6,4,9};
    int output[8];
    int input2[32] = {};
    //input2[0] = 31;
    int timeOfOWF2[ARRAY_LENGTH] = {6,3,2,8,7,5,3,4,3,6};
    int output2[8];

    int timeOfOWF3[ARRAY_LENGTH] = {4,4,3,3,3,3,6,2,1,3};
    int output3[8];
    srand(time(NULL));
    for(int i = 0 ; i < 10 ; i++){
        input[i] = rand() % LIMIT;
        input2[i] = input[i];
    }

    hash_based_encryption(input, timeOfOWF, output, ARRAY_LENGTH);
    printf("address: %p\n",&input);
    //printf("output: is %d\n",output[1]);
    hash_based_encryption(input2, timeOfOWF2, output2, ARRAY_LENGTH);
    printf("address: %p\n",&input2);
    hash_based_encryption(output2, timeOfOWF3, output3, ARRAY_LENGTH);
    printf("\n");
    //printf("output3: is %d\n",output3[1]);
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