#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define BUFFER_SIZE 5
#define DICT_SIZE 4
#define NUM_WORKERS 3
const char *input_buffer[BUFFER_SIZE];
const char *result_buffer[BUFFER_SIZE];
int result_status[BUFFER_SIZE];
int count = 0;
int result_count = 0;
int in = 0;
int out = 0;
int words_processed = 0;
int total_words = 5;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_done = PTHREAD_COND_INITIALIZER;
const char *dictionary[DICT_SIZE] = {"apple", "banana", "cherry", "date"};
void *spell_check(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count == 0 && words_processed < total_words) {
            pthread_cond_wait(&cond_full, &mutex);
        }
        if (words_processed >= total_words && count == 0) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        const char *word = input_buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
        int found = 0;
        for (int i = 0; i < DICT_SIZE; i++) {
            if (strcmp(word, dictionary[i]) == 0) {
                found = 1;
                break;
            }
        }
        pthread_mutex_lock(&mutex);
        result_buffer[result_count] = word;
        result_status[result_count] = found;
        result_count++;
        words_processed++;       
        if (words_processed == total_words) {
            pthread_cond_signal(&cond_done);
            pthread_cond_broadcast(&cond_full);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main() {
    const char *words_to_check[] = {"apple", "orange", "banana", "grape", "cherry"};
    pthread_t workers[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_create(&workers[i], NULL, spell_check, NULL);
    }
    for (int i = 0; i < total_words; i++) {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_empty, &mutex);
        }
        input_buffer[in] = words_to_check[i];
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    while (words_processed < total_words) {
        pthread_cond_wait(&cond_done, &mutex);
    }
    printf("--- Spell Check Results ---\n");
    for (int i = 0; i < result_count; i++) {
        printf("Word: %-8s | Status: %s\n", 
               result_buffer[i], 
               result_status[i] ? "Correct" : "Incorrect");
    }
    pthread_mutex_unlock(&mutex);
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(workers[i], NULL);
    }
    return 0;
}

