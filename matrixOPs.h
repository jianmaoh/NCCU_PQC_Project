#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 64
#define M 36
#define W 2


// define it as a f function in WOTS to transform a vector to a matrix
void vectorToMatrix(int vector[M], int matrix[N][M]) {
    for (int j = 0; j < M; j++) {
        int num = vector[j];
        for (int i = N - 1; i >= 0; i--) {
            matrix[i][j] = num % 2;
            num = num / 2;
        }
    }
}

// print a matrix
void printMatrix(int matrix[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


// define it as a F function in WOTS to genearate a new vector
