#include <stdio.h>
//#include "matrixOPs.h"
#include "mathOPs.h"
#include <stdlib.h>
#include <time.h>

// wots裡的摘要長度  sig(SK,d)  VK: verification key, SK: signature key, d: message摘要
// 論文裡有多丟一個VK但應該不需要 ver時再丟進去就好
//t1 = ceil(n/w) n=32 w=2 t1=16
//t2 = floor(log2(t1(2^w-1))/w))+1 t1=16 w=2 t2=
#define N 32
// 論文裡的n
#define M 19

int main() {
    int matrix[N][M] = {0};
    int matrix2[N][M] = {0};
    int message[N]={1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,1,0,1,1};
    int length = sizeof(message) / sizeof(message[0]);
    printf("Length of the array is %d\n", length);

   
    struct timespec start, end;
    long seconds, nanoseconds;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    struct vectorPair pair = keyGen();
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    elapsed = seconds + nanoseconds * 1e-9;
    printf("W-OTS Key generation time: %.9f\n", elapsed);

    printf("The signature key X as a vector is\n");
    for (size_t i = 0; i < M; i++)
    {
        printf("%d ", pair.X[i]);
    }
    printf("\n");
    printf("The signature key Y as a vector is\n");
    for (size_t i = 0; i < M; i++)
    {
        printf("%d ", pair.Y[i]);
    }
    printf("\n");
   
    clock_gettime(CLOCK_MONOTONIC, &start);

    int *signature = sig(pair.X,message);

    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    elapsed = seconds + nanoseconds * 1e-9;
    printf("W-OTS Signature generation time: %.9f\n", elapsed);

    printf("The signature is ");
    for (size_t i = 0; i < M; i++)
    {
        printf("%d ", signature[i]);
    }
    printf("\n");

    clock_gettime(CLOCK_MONOTONIC, &start);

    if(ver(signature,pair.Y,message)){
        printf("The signature is valid\n");
    }else{
        printf("The signature is invalid\n");
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    seconds = end.tv_sec - start.tv_sec;
    nanoseconds = end.tv_nsec - start.tv_nsec;
    elapsed = seconds + nanoseconds * 1e-9;
    printf("W-OTS Ver time: %.9f\n", elapsed);

    free(signature);  

    int gu[] = {1, 5, 13, 19}; // vector u
    int f[] = {-1, 1, 0, 0, 1}; // f(x) 
    int gu_size = sizeof(gu) / sizeof(gu[0]); // size of vector u
    int f_size = sizeof(f) / sizeof(f[0]); // init_size of f(x)
    int result[MAX_SIZE][MAX_SIZE] = {0}; // result matrix

    FRD(gu, gu_size, f,f_size ,result); 

    for (int i = 0; i < gu_size; i++) {
        for (int j = 0; j < gu_size; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
 
    return 0;
}