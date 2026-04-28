#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
int main() {
    int n,seed=105;

    printf("Enter number size of matrix: ");
    scanf("%d", &n);

    int *mat1 = (int*)malloc(sizeof(int)*n*n);
    int *mat2 = (int*)malloc(sizeof(int)*n*n);
    int **res = (int**)malloc(sizeof(int*)*n);
    for (int i = 0; i < n; i++){
        res[i]= (int*)malloc(sizeof(int)*n);
    }
    srand(seed);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat1[i*n + j]=rand()%10;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat2[i*n + j]=rand()%10;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i][j]=mat1[i*n + j] + mat2[i*n + j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i][j]=mat1[i*n + j] - mat2[i*n + j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i][j]=0;
            for (int k = 0; k < n; k++) {
                res[i][j] += mat1[i*n + k] * mat2[k*n + j];
            }
        }
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
                res[i][j]=mat1[j*n+i];


    clock_gettime(CLOCK_MONOTONIC, &end);
    double stime=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nSerial Execution Time: %lf seconds\n", stime);
    return 0;
}
