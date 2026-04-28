#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
#include <omp.h>
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
int main() {
    std::queue<std::string> shared_queue;
    bool producers_done = false;
    omp_lock_t queue_lock;
    omp_init_lock(&queue_lock);
    std::string files[NUM_PRODUCERS] = {"file1.txt", "file2.txt"};
    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        int tid = omp_get_thread_num();
        if (tid < NUM_PRODUCERS) {
            std::ifstream file(files[tid]);
            std::string line;
            if (file.is_open()) {
                while (std::getline(file, line)) {
                    omp_set_lock(&queue_lock);
                    shared_queue.push(line);
                    omp_unset_lock(&queue_lock);
                }
                file.close();
            }
            #pragma omp barrier
            if (tid == 0) producers_done = true;
        } 
        else {
            while (true) {
                std::string line = "";
                bool found = false;

                omp_set_lock(&queue_lock);
                if (!shared_queue.empty()) {
                    line = shared_queue.front();
                    shared_queue.pop();
                    found = true;
                }
                omp_unset_lock(&queue_lock);

                if (found) {
                    std::stringstream ss(line);
                    std::string token;
                    while (ss >> token) {
                        #pragma omp critical
                        std::cout << token << std::endl;
                    }
                } else if (producers_done) {
                    break; 
                }
            }
        }
    }
    omp_destroy_lock(&queue_lock);
    return 0;
}

