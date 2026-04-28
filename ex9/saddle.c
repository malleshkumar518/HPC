#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int r, c, count = 0;
    printf("Enter rows and columns: ");
    scanf("%d %d", &r, &c);
    int **matrix = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++)
        matrix[i] = (int *)malloc(c * sizeof(int));
    printf("Enter matrix elements:\n");
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            scanf("%d", &matrix[i][j]);
    #pragma omp parallel for reduction(+:count)
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int val = matrix[i][j];
            int min = 1;
            int max = 1;
            for (int k = 0; k < c; k++) {
                if (matrix[i][k] < val) {
                    min = 0;
                    break;
                }
            }
            if (min) {
                for (int k = 0; k < r; k++) {
                    if (matrix[k][j] > val) {
                        max = 0;
                        break;
                    }
                }
            } else {
                max = 0;
            }
            if (min && max) {
                count++;
            }
        }
    }
    printf("Number of saddle points: %d\n", count);
    for (int i = 0; i < r; i++) free(matrix[i]);
    free(matrix);
    return 0;
}

