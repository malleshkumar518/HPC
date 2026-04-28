#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() 
{
    int m, n, p, i, j, k;
    printf("Enter rows (m): "); 
    scanf("%d", &m);
    printf("Enter cols/rows (n): "); 
    scanf("%d", &n);
    printf("Enter cols (p): "); 
    scanf("%d", &p);
    int *a = (int *)malloc(m * n * sizeof(int));
    int *b = (int *)malloc(n * p * sizeof(int));
    int *rs = (int *)malloc(m * p * sizeof(int));
    srand(time(0));

    for(i=0; i<m*n; i++) 
       a[i] = 10 + rand() % 90;
    for(i=0; i<n*p; i++) 
       b[i] = 10 + rand() % 90;

    if(m <= 10 && n <= 10 && p <= 10){	 
       printf("Matrix 1\n");
       for(i = 0; i < m; i++){
	  for(j = 0; j < p; j++){
	     printf("%d ", a[i * n + j]);
	  }
	  printf("\n");
       }
       
       printf("\nMatrix 2\n");
       for(i = 0; i < m; i++){
	  for(j = 0; j < p; j++){
	     printf("%d ", b[i * n + j]);
	  }
	  printf("\n");
	  printf("\n");
       }
    }

    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for(i=0; i<m; i++) {
        for(j=0; j<p; j++) {
            rs[i*p+j] = 0;
            for(k=0; k<n; k++) {
                rs[i*p+j] += a[i*n+k] * b[k*p+j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);
    double t_ser = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
     
    if(m <= 10 && n <= 10 && p <= 10){
       printf("Resultant Matrix\n");
       for(i = 0; i < m; i++){
	  for(j = 0; j < p; j++){
	     printf("%d ", rs[i * n + j]);
	  }
	  printf("\n");
       }
    }
    if(m<=10&&n<=10)
    printf("\n------------------------------------------------");
    printf("\nInput Size: %dx%d multiplied by %dx%d", m, n, n, p);
    printf("\n------------------------------------------------");
    printf("\nSerial Execution Time   : %lf seconds", t_ser);
    printf("\n------------------------------------------------\n");
    free(a); free(b); free(rs);
    return 0;
}
