#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define n 4
#define m 4
#define w 2
#define l 2
#define t 2
#define q 7

// Assuming D is a function that generates a random number from a normal distribution

double gaussian_rand(double mean, double std_dev)
{
    double u = (double)rand() / RAND_MAX;
    double v = (double)rand() / RAND_MAX;
    double x = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    return mean + std_dev * x;
}

void KeyGen()
{
    int A[n][m], R1[m][w], R2[m][l], C[n][t], B1[n][w], B2[n][l];

    srand(time(0)); // seed for random number generation

    // Sample A nxm
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            A[i][j] = rand() % q; // generate random number from 0 to q-1
        }
    }

    // Sample R1 and R2
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < w; j++)
        {
            R1[i][j] = (int)gaussian_rand(0, sqrt(log(n)));
        }
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < l; j++)
        {
            R2[i][j] = (int)gaussian_rand(0, sqrt(log(n)));
        }
    }

    // for(int i = 0; i < m; ++i) {
    //     for(int j = 0; j < w; ++j)
    //         printf("%d\n", R1[i][j]);
    // }

    // Sample C
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < t; j++)
        {
            C[i][j] = rand() % q; // generate random number from 0 to q-1
        }
    }

    // B1
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < w; ++j) {
            B1[i][j] = 0;
            for(int k = 0; k < m; ++k) {
                B1[i][j] += A[i][k] * R1[k][j];
            }
        }
    }

    // B2
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < l; ++j) {
            B2[i][j] = 0;
            for(int k = 0; k < m; ++k) {
                B2[i][j] += A[i][k] * R2[k][j];
            }
        }
    }

    // Output pk and sk
    printf("A:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("B1:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < w; j++)
        {
            printf("%d ", B1[i][j]);
        }
        printf("\n");
    }

    printf("B2:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < l; j++)
        {
            printf("%d ", B1[i][j]);
        }
        printf("\n");
    }

    printf("C:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < t; j++)
        {
            printf("%d ", C[i][j]); 
        }
        printf("\n");
    }

    printf("R1:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < w; j++)
        {
            printf("%d ", R1[i][j]);
        }
        printf("\n");
    }

    printf("R2:\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < l; j++)
        {
            printf("%d ", R2[i][j]);
        }
        printf("\n");
    }

    // pk = (A, B1, B2, C)
    // sk = (R1, R2)
}

int main()
{
    KeyGen();
    return 0;
}