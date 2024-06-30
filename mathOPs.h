#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "matrixOPs.h"
#include <string.h>
#include <stdbool.h>
//#include <openssl/sha.h>

#define N 64 // n in the paper
#define M 36 // t in the paper
#define W 2  // w in the paper
#define MAX_SIZE 4

unsigned int APHash(const int input){
    
   unsigned int hash = 3777171;
   unsigned int i = 7;
   hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (input) * (hash >> 3)) :
                               (~((hash << 11) + ((input) ^ (hash >> 5))));
   return hash % 1048576;
}

// output two vectors  
struct vectorPair {
    int X[M];
    int Y[M];
};

// generate a random vector
int* generateRandomVector(int n) {
    int* result = (int*)malloc(M * sizeof(int));
    int upperLimit = pow(2, n);
    srand(time(NULL));  // 使用當前時間作為隨機數生成器的種子
    for (int i = 0; i < M; i++) {
        result[i] = rand() % upperLimit;
    }
    return result;
}

//the one way f function in wots key generation
int* OWFInKeyGen(int *vector,int length) { 
    int times = pow(2, W) - 1;
    int limit = pow(2, N);
    int* output = malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        output[i] = vector[i];
        for (int j = 0; j < times; j++) {
            output[i] = APHash(output[i]);
        }
    }
    return output;
}


// key generation in wots
struct vectorPair keyGen() {
    struct vectorPair pair;
    int* tempX = generateRandomVector(N);
    memcpy(pair.X, tempX, M * sizeof(int));
    free(tempX);
    int* tempY = OWFInKeyGen(pair.X, M);
    memcpy(pair.Y, tempY, M * sizeof(int));
    free(tempY);
    return pair;
}


// mix two arrays (frontArr and backArr) into one array
int *mixArray(int *frontArr, int *backArr, int lenghOfArr1, int lengthOfArr2) {
    int *result = (int *)malloc((lenghOfArr1 + lengthOfArr2) * sizeof(int));
    memcpy(result, frontArr, lenghOfArr1 * sizeof(int));
    memcpy(result + lenghOfArr1, backArr, lengthOfArr2 * sizeof(int)); 
    return result;  
}

// convert decimal to binary array
int *decimal_to_binary(int decimal, int *length) {
    *length = floor(log2(decimal)) + 1;
    int *binary = (int *)malloc(*length * sizeof(int));
    for (int i = *length - 1; i >= 0; i--) {
        binary[i] = decimal % 2;
        decimal /= 2;
    }
    return binary;
}

// split the array into blocks of size w and convert each block to decimal
int *split_and_convert(int *arr, int length, int w) {
    int num_blocks = length / w;
    int *result = (int *)malloc(num_blocks * sizeof(int));
    for (int i = 0; i < num_blocks; i++) {
        int block_value = 0;
        for (int j = 0; j < w; j++) {
            block_value = block_value * 2 + arr[i * w + j];
        }
        result[i] = block_value;
    }
    return result;
}

// the one way f function in wots signature
int* OWFInSig(int vector[M],int b[M]) { 
    int* result = malloc(M * sizeof(int));
    int limit = pow(2, N);
    for (int i = 0; i < M; i++) {
        result[i] = vector[i];
        int times = b[i];
        for (int j = 0; j < times; j++) {
            result[i] = APHash(result[i]);
        }
    }
    return result;
}

// the one way f function in wots verification
int *OWFInVer(int vector[M],int b[M]) { 
    int times = pow(2, W) - 1;
    int* result = malloc(M * sizeof(int));
    int limit = pow(2, N);
    for (int i = 0; i < M; i++) {
        result[i] = vector[i];
        for (int j = 0; j < times - b[i]; j++) {

            result[i] = APHash(result[i]);
        } 
    }
    return result;
}



// digest's lenght = N 
// input digest and do the operations in signature
// output a vector that can be used in signature
int *sigOpArr(int *digest){ // 
    // fill the arr with 0 to make it can be divided by W
    int padding;
    if (N % W == 0) {
        padding = 0;
    }else {
        padding = W - (N % W); 
    }
    int *fillArr = (int *)malloc(padding * sizeof(int));
    memset(fillArr, 0, padding * sizeof(int)); // fillArr = [0]
    int *filledDigest = (int *)malloc((N + padding) * sizeof(int));
    filledDigest = mixArray(fillArr, digest, padding ,N ); // filledDigest = [0101] 
    // then we start to compute checksum
    int *convertedDigest = split_and_convert(filledDigest, (N + padding), W);
    int powerOfTwo = pow(2, W);
    int checksum = 0;
    for (int i = 0; i < (N + padding)/ W; i++) {
        checksum += powerOfTwo  - convertedDigest[i];
    }
    int checksum_length;
    int *checksumArr = decimal_to_binary(checksum, &checksum_length);
    //then start to fill the checksum array with 0 to make it can be divided by W
    int padding2;
    if (checksum_length % W == 0) {
        padding2 = 0;
    }else {
        padding2 = W - (checksum_length % W); // padding = 1
    }
    int *fillArr2 = (int *)malloc(padding2 * sizeof(int));
    memset(fillArr2, 0, padding2 * sizeof(int)); // fillArr = [0]   
    int *filledChecksum = (int *)malloc((checksum_length + padding2) * sizeof(int));
    filledChecksum = mixArray(fillArr2, checksumArr, padding2 ,checksum_length ); // filledDigest = [0110]
    // then divide it by w 
    int *convertedChecksum = split_and_convert(filledChecksum, (checksum_length + padding2), W);
    //then start to merge the convertedDigest and convertedChecksum
    int *result = mixArray(convertedDigest, convertedChecksum, (N + padding)/W, (checksum_length + padding2)/W);
    free(fillArr);
    free(filledDigest);
    free(convertedDigest);
    free(checksumArr);
    free(fillArr2);
    free(filledChecksum);
    free(convertedChecksum);
    return result; 
}

//SIG in wots
// sig(SK, d)  VK: verification key, SK: signature key, d: message摘要
int *sig(int X[M],int *digest){
    int *timeOfOWF = sigOpArr(digest);
    int *sig = OWFInSig(X,timeOfOWF);
    return sig;
}


//VER in wots
// ver(SK, VK, d)  VK: verification key, SK: signature key, d: message摘要
// 把SK和digest丟進去，再把結果和VK比對
bool ver(int *sign,int Y[M],int *digest){
    int *timeOfOWF = sigOpArr(digest);
    int *ver = OWFInVer(sign,timeOfOWF);
    printf("ver is ");
    for (int i = 0; i < M; i++){
        printf("%d ", ver[i]);
    }
    printf("\n");

    for (int i = 0; i < M; i++){
        if (Y[i] != ver[i]){
            return false;
        }
    }
    return true;
}

// get polynomial remainder 
// polydiv(poly1, size of poly1, poly2, size of poly2, remainder)
void polydiv(int *poly1, int size1, int *poly2, int size2, int *remainder) {
    int i, j;
    memcpy(remainder, poly1, size1 * sizeof(int));
    for (i = size1 - size2; i >= 0; i--) {
        int coef = remainder[i + size2 - 1] / poly2[size2 - 1];
        for (j = 0; j < size2; j++) {
            remainder[i + j] -= coef * poly2[j];
        }
    }
}

// prepend zero to the array
// prependZero(inputarray, size of it, number of zero to add, result array)
void prependZero(int *array, int size, int zero_num,int *result) {
    int i;
    for (i = 0; i < zero_num; i++) {
        result[i] = 0;
    }
    for (i = 0; i < size; i++) {
        result[i + zero_num] = array[i];
    }
}

// full rank encoding function
//FRD(vector u, f(x), size of vector u, size of f(x), result matrix)
void FRD(int *gu, int gu_size, int *f, int f_size, int result[MAX_SIZE][MAX_SIZE]) {
    
    int temp[2 * MAX_SIZE] = {0};
    memcpy(result[0], gu, gu_size * sizeof(int));

    for(int i = 1; i < gu_size; i++) {
        
        int temp2[MAX_SIZE] = {0};
        prependZero(gu, (gu_size + i - 1), i, temp);
        polydiv(temp, (gu_size + i), f, f_size, temp2);
        memcpy(result[i], temp2, gu_size * sizeof(int));
    }
    
}



