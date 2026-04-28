#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define QUEUE_SIZE 5
#define SEQ_LEN 10
int queue[QUEUE_SIZE][SEQ_LEN];
int head = 0, tail = 0, count = 0;
omp_lock_t q_lock;
double g_sum = 0, g_sq_sum = 0;
long g_total_elements = 0;
void produce() {
    int s[SEQ_LEN];
    for (int i = 0; i < SEQ_LEN; i++) s[i] = rand() % 100;
    int placed = 0;
    while (!placed) {
        omp_set_lock(&q_lock);
        if (count < QUEUE_SIZE) {
            for (int i = 0; i < SEQ_LEN; i++) queue[tail][i] = s[i];
            tail = (tail + 1) % QUEUE_SIZE;
            count++;
            placed = 1;
        }
        omp_unset_lock(&q_lock);
    }
}
void consume() {
    int s[SEQ_LEN];
    int grabbed = 0;
    while (!grabbed) {
        omp_set_lock(&q_lock);
        if (count > 0) {
            for (int i = 0; i < SEQ_LEN; i++) s[i] = queue[head][i];
            head = (head + 1) % QUEUE_SIZE;
            count--;
            grabbed = 1;
        }
        omp_unset_lock(&q_lock);
    }
    double l_sum = 0, l_sq = 0;
    for (int i = 0; i < SEQ_LEN; i++) {
        l_sum += s[i];
        l_sq += (s[i] * s[i]);
    }
    #pragma omp critical
    {
        double m = l_sum / SEQ_LEN;
        double v = (l_sq / SEQ_LEN) - (m * m);
        printf("Mean: %.2f, Var: %.2f\n", m, v);
    }
    #pragma omp atomic
    g_sum += l_sum;
    #pragma omp atomic
    g_sq_sum += l_sq;
    #pragma omp atomic
    g_total_elements += SEQ_LEN;
}
int main() {
    omp_init_lock(&q_lock);
    #pragma omp parallel sections
    {
        #pragma omp section
        for(int i=0; i<20; i++) produce();
        #pragma omp section
        for(int i=0; i<20; i++) consume();
    }
    printf("Global Mean: %.2f\n", g_sum / g_total_elements);
    omp_destroy_lock(&q_lock);
    return 0;
}

