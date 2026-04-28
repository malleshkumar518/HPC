#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arr[100];
int n, num_threads;
int partial_sum[10];

void* sumArray(void* arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * (n / num_threads);
    int end = (thread_id == num_threads - 1) ? n : start + (n / num_threads);

    partial_sum[thread_id] = 0;
    
    for (int i = start; i < end; i++) {
        partial_sum[thread_id] += arr[i];
    }
    
    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_id[10];
    int total_sum = 0;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter Array Elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter Number of threads: ");
    scanf("%d", &num_threads);
    for (int i = 0; i < num_threads; i++) {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, sumArray, &thread_id[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d partial sum: %d\n", i, partial_sum[i]);
        total_sum += partial_sum[i];
    }

    printf("Total Sum is %d\n", total_sum);
    return 0;
}

