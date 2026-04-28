#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int m, n, p, i, j, k;

    printf("Enter rows (m): "); scanf("%d", &m);
    printf("Enter cols/rows (n): "); scanf("%d", &n);
    printf("Enter cols (p): "); scanf("%d", &p);

    // Shared memory allocation
    int sz = (m*n + n*p + m*p) * sizeof(int);
    int sid = shmget(IPC_PRIVATE, sz, IPC_CREAT | 0666);
    int *mem = shmat(sid, 0, 0);

    int tid = shmget(IPC_PRIVATE, m * sizeof(double), IPC_CREAT | 0666);
    double *tmem = shmat(tid, 0, 0);

    int *a = mem;
    int *b = mem + m*n;
    int *rp = mem + m*n + n*p;

    srand(time(0));
    for(i=0; i<m*n; i++) a[i] = 10 + rand() % 90;
    for(i=0; i<n*p; i++) b[i] = 10 + rand() % 90;

    // Parallel Matrix Multiplication using fork
    for(i=0; i<m; i++) {
        if(fork() == 0) {
            struct timespec st, en;
            clock_gettime(CLOCK_MONOTONIC, &st);

            for(j=0; j<p; j++) {
                rp[i*p+j] = 0;
                for(k=0; k<n; k++) {
                    rp[i*p+j] += a[i*n+k] * b[k*p+j];
                }
            }

            clock_gettime(CLOCK_MONOTONIC, &en);
            tmem[i] = (en.tv_sec - st.tv_sec) + (en.tv_nsec - st.tv_nsec) / 1e9;
            exit(0);
        }
    }

    for(i=0; i<m; i++) wait(NULL);

    double t_par = -1;
    for(i=0; i<m; i++) {
        if(t_par < tmem[i]) t_par = tmem[i];
    }
    if(m<=10 && n<=10 && p<=10){
       printf("Matrix A\n");
       for(i=0;i<m;i++){
          for(j=0;j<n;j++){
             printf("%d\t",a[i*n+j]);
          }
          printf("\n");
       }
       printf("Matrix B\n");
       for(i=0;i<n;i++){
          for(j=0;j<p;j++){
             printf("%d\t",b[i*p+j]);
          }
          printf("\n");
       }
       printf("Resultant Matrix:\n");
       for(i=0; i<m; i++){
          for(j=0; j<p; j++){
             printf("%d\t",rp[i*p+j]);
          }
          printf("\n");
       }
    }
    printf("\n");
    printf("\nInput Size: %dx%d multiplied by %dx%d", m, n, n, p);
    printf("\n");
    printf("\nParallel Execution Time : %lf seconds", t_par);
    printf("\n");
    shmdt(mem); shmdt(tmem);
    shmctl(sid, IPC_RMID, NULL); shmctl(tid, IPC_RMID, NULL);
    return 0;
}
