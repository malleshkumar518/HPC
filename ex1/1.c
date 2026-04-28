#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
void print_matrix(const char* name, int* mat, int rows, int cols) 
{
    printf("\nMatrix %s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", mat[i * cols + j]);
        }
        printf("\n");
    }
}
void run_serial(int m) 
{
    int n = m, p = m, mlp = 10;
    int size = m * n;
    int *shm = (int *)malloc((size * 3) * sizeof(int));
    if (shm == NULL) { perror("malloc failed"); exit(1); }
    srand(time(NULL));
    int *mat1 = shm;
    int *mat2 = shm + size;
    int *res  = shm + (size * 2);
    for (int i = 0; i < size; i++) mat1[i] = rand() % mlp;
    for (int i = 0; i < size; i++) mat2[i] = rand() % mlp;
    if (m<=10){
        print_matrix("A", mat1, m, n);
        print_matrix("B", mat2, n, p);
     }
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < p; j++) {
            res[i * p + j] = 0;
            for(int k = 0; k < n; k++) {
                res[i * p + j] += mat1[i * n + k] * mat2[k * p + j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (m<=10){
        print_matrix("Result (Serial)", res, m, p);
    }
    double stime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("\nSerial Execution Time: %lf seconds\n", stime);
    free(shm);
}
void run_parallel(int m) 
{
    int n = m, p = m, mlp = 10;
    srand(time(NULL));
    int shmid = shmget(IPC_PRIVATE, (m*n + n*p + m*p) * sizeof(int), IPC_CREAT | 0666);
    int shmtid = shmget(IPC_PRIVATE, m * sizeof(double), IPC_CREAT | 0666);
    int *shm = (int *)shmat(shmid, 0, 0);
    double *shmt = (double *)shmat(shmtid, 0, 0);
    int *mat1 = shm;
    int *mat2 = shm + m*n;
    int *res  = shm + m*n + n*p;
    for (int i = 0; i < m*n; i++) mat1[i] = rand() % mlp;
    for (int i = 0; i < n*p; i++) mat2[i] = rand() % mlp;
    if (m<=10){
        print_matrix("A", mat1, m, n);
        print_matrix("B", mat2, n, p);
     }
    for (int i = 0; i < m; i++) {
        pid_t pid = fork(); 
        if (pid == 0) {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = 0; j < p; j++) {
                res[i * p + j] = 0;
                for (int k = 0; k < n; k++) {
                    res[i * p + j] += mat1[i * n + k] * mat2[k * p + j];
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            shmt[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            exit(0);
        }
    }
    for (int i = 0; i < m; i++) wait(NULL); 
    if(m<=10){
        print_matrix("Result (Parallel)", res, m, p);
    }
    double max_ptime = 0;
    for(int i = 0; i < m; i++) if(shmt[i] > max_ptime) max_ptime = shmt[i];
    printf("\nParallel Execution Time (Longest Child): %lf seconds\n", max_ptime);
    shmdt(shm); shmctl(shmid, IPC_RMID, NULL);
    shmdt(shmt); shmctl(shmtid, IPC_RMID, NULL);
}
int main() 
{
    int choice, m;
    printf("\n--- Matrix Multiplication Menu ---\n");
    printf("Enter matrix order (m x m): ");
    scanf("%d", &m);
    run_serial(m);
    run_parallel(m);
    return 0;
}
