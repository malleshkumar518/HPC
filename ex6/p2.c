#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#define BUFFER_SIZE 5
typedef struct 
{
    int a, b;
    char op;
    int is_last; 
} WorkItem;
WorkItem work_buffer[BUFFER_SIZE];
int in = 0, out = 0;
int n = 10;
sem_t empty_slots;  
sem_t full_slots;   
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
void* compute_worker(void* arg) 
{
    char target_op = *(char*)arg;
    FILE *file;
    while (1) 
    {
        sem_wait(&full_slots); 
        pthread_mutex_lock(&buffer_mutex);
        if (work_buffer[out].op == target_op) 
	{
            WorkItem task = work_buffer[out];
            out = (out + 1) % BUFFER_SIZE;            
            int res = 0;
            if (task.op == '+') res = task.a + task.b;
            else if (task.op == '-') res = task.a - task.b;
            else if (task.op == '*') res = task.a * task.b;
            else if (task.op == '/') res = (task.b != 0) ? task.a / task.b : 0;
            file = fopen("p2.txt", "a");
            if (file) 
	    {
                fprintf(file, "Thread [%c]: %d %c %d = %d\n", target_op, task.a, task.op, task.b, res);
                fclose(file);
            }
           // printf("Thread [%c]: Processed %d %c %d\n", target_op, task.a, task.op, task.b, res);
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&empty_slots); 
        } else {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&full_slots); 
            usleep(1000); 
        }
    }
    return NULL;
}
int main() 
{
    pthread_t threads[4];
    char ops[4] = {'+', '-', '*', '/'};   
    srand(time(NULL)); 
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, compute_worker, &ops[i]);
    }
    for (int j = 0; j < n; j++) {
        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);
        work_buffer[in].a = rand() % 50;
        work_buffer[in].b = rand() % 50;
        work_buffer[in].op = ops[rand() % 4];
        printf("Server: Dispatched %d %c %d\n", work_buffer[in].a, work_buffer[in].op, work_buffer[in].b);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);
        sleep(1); 
    }
    printf("Server: All %d tasks dispatched.\n", n);
    return 0;
}
