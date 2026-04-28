#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SUBMISSIONS 10
#define TARGET_RESULTS 10

int pending_submissions = 0;
int evaluation_done = 0;
int results_count = 0;
int keep_running = 1;

pthread_mutex_t lock;
pthread_cond_t cond_submission;
pthread_cond_t cond_space;
pthread_cond_t cond_result;

void* submitter(void* arg) {
    while (keep_running) {
        sleep(1);
        pthread_mutex_lock(&lock);
        while (keep_running && pending_submissions >= MAX_SUBMISSIONS) {
            pthread_cond_wait(&cond_space, &lock);
        }
        if (!keep_running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pending_submissions++;
        printf("Submitter: Pending = %d\n", pending_submissions);
        pthread_cond_signal(&cond_submission);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* evaluator(void* arg) {
    while (keep_running) {
        pthread_mutex_lock(&lock);
        while (keep_running && pending_submissions == 0) {
            pthread_cond_wait(&cond_submission, &lock);
        }
        if (!keep_running) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pending_submissions--;
        printf("Evaluator: Pending = %d\n", pending_submissions);
        evaluation_done = 1;
        pthread_cond_signal(&cond_space);
        pthread_cond_signal(&cond_result);
        pthread_mutex_unlock(&lock);
        sleep(2);
    }
    return NULL;
}

void* monitor(void* arg) {
    while (keep_running) {
        printf("Monitor: Pending = %d\n", pending_submissions);
        usleep(500000);
    }
    return NULL;
}

void* publisher(void* arg) {
    while (keep_running) {
        pthread_mutex_lock(&lock);
        while (keep_running && evaluation_done == 0) {
            pthread_cond_wait(&cond_result, &lock);
        }
        if (keep_running) {
            results_count++;
            printf("Publisher: Result #%d published\n", results_count);
            evaluation_done = 0;
            if (results_count >= TARGET_RESULTS) {
                keep_running = 0;
                pthread_cond_broadcast(&cond_submission);
                pthread_cond_broadcast(&cond_space);
                pthread_cond_broadcast(&cond_result);
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_submission, NULL);
    pthread_cond_init(&cond_space, NULL);
    pthread_cond_init(&cond_result, NULL);

    pthread_create(&t1, NULL, submitter, NULL);
    pthread_create(&t2, NULL, evaluator, NULL);
    pthread_create(&t3, NULL, monitor, NULL);
    pthread_create(&t4, NULL, publisher, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_submission);
    pthread_cond_destroy(&cond_space);
    pthread_cond_destroy(&cond_result);

    return 0;
}

