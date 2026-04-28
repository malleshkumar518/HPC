#include <stdio.h>
#include <pthread.h>
#define MAX 10
int A[MAX][MAX], B[MAX][MAX], Sum[MAX][MAX], Mul[MAX][MAX]={0};
int r, c;
void* add_matrices(void* arg) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            Sum[i][j] = A[i][j] + B[i][j];
        }
    }
    return NULL;
}
void* mul_matrices(void* arg) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
	   for (int k=0; k<r; k++){
            Mul[i][j] += A[i][k] * B[k][j];       
	   }
	}
    }
    return NULL;
}
int main() {
    pthread_t thread1, thread2;
    printf("Enter the rows and columns: ");
    scanf("%d %d", &r, &c);
    printf("Enter the values for Matrix A:\n");
    for(int i=0; i<r; i++){
       for(int j=0; j<c; j++){
	  scanf("%d", &A[i][j]);
       }
    }
    printf("Enter the values for Matrix B:\n");
    for(int i=0; i<r; i++){
       for(int j=0; j<c; j++){
	  scanf("%d", &B[i][j]);
       }
    }
    pthread_create(&thread1, NULL, add_matrices, NULL);
    pthread_create(&thread2, NULL, mul_matrices, NULL);
    pthread_join(thread1, NULL);
    if (r!=c){
       printf("Multiplcation is not possible\n");
       return 0;
    }
    pthread_join(thread2, NULL);
    printf("\nAddition Result:\n");
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) printf("%d ", Sum[i][j]);
        printf("\n");
    }
    printf("\nMultiplication Result:\n");
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) printf("%d ", Mul[i][j]);
        printf("\n");
    }
    return 0;
}
