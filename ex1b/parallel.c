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
    int n;
    int seed=105;
    printf("Enter number size of matrix: ");
    scanf("%d", &n);

    int shmsize = (2*n*n) * sizeof(int);
    int shmid = shmget(IPC_PRIVATE, shmsize, IPC_CREAT| IPC_EXCL | 0666);

    int shm_time = 4 * sizeof(double);
    int shmtid = shmget(IPC_PRIVATE, shm_time, IPC_CREAT | IPC_EXCL | 0666);

    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    int *shm = shmat(shmid, 0, 0);
    double *shmt = shmat(shmtid, 0, 0);

    if (shm == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    int *mat1 = shm;
    int *mat2 = shm + n*n;
    srand(seed);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat1[i*n + j]=rand()%10;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat2[i*n + j]=rand()%10;


    if (fork() == 0) {
        int *res=(int*)malloc(sizeof(int)*(n*n));
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                res[i*n + j]=mat1[i*n + j] + mat2[i*n + j];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double ptime=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        shmt[0]=ptime;
        printf("Addition time: %lf seconds\n",end.tv_nsec,ptime);
        exit(0);
    }

    if (fork() == 0) {
        int *res=(int*)malloc(sizeof(int)*(n*n));
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                res[i*n + j]=mat1[i*n + j] - mat2[i*n + j];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double ptime=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        shmt[1]=ptime;
        printf("Subtraction time: %lf seconds\n",end.tv_nsec,ptime);
        exit(0);
    }

    if (fork() == 0) {
        int *res=(int*)malloc(sizeof(int)*(n*n));
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                res[i*n + j]=0;
                for (int k = 0; k < n; k++) {
                     res[i*n + j] += mat1[i*n + k] * mat2[k*n + j];
                }
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double ptime=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        shmt[2]=ptime;
        printf("Multiplication time: %lf seconds\n",end.tv_nsec,ptime);
        exit(0);
    }

    if (fork() == 0) {
        int **res=(int**)malloc(sizeof(int*)*n);
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0 ; i < n; i++){
            res[i]=(int*)malloc(sizeof(int)*n);
            for(int j = 0; j < n; j++){
                res[i][j]=mat1[j*n + i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        double ptime=(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        shmt[3]=ptime;
        printf("Determinant time: %lf seconds\n",end.tv_nsec,ptime);
        exit(0);
    }

    for (int i = 0; i < 4; i++)
        wait(NULL);

    double tmp=-1;
    for(int i=0;i<4;i++){
        if(tmp < shmt[i])
                tmp=shmt[i];
    }
    printf("\nParallel Execution Time: %lf seconds\n", tmp);

    shmdt(shm);
    shmdt(shmt);
    shmctl(shmid, IPC_RMID, 0);
    shmctl(shmtid, IPC_RMID, 0);
    return 0;
}
