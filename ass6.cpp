#include <stdio.h>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

// define a mutex variable
pthread_mutex_t mutex;


// shared variable
int shared_data = 0;
bool condition = false;
unsigned int seed = time(NULL);

// function to be executed by thread1
void* writing_data(void* arg) {
    int thread_id = *(int*)arg;



    for (int i = 1; i <= 10; i++) {

        // checks if it can write new value into shared_data
        while (condition) {}

        // lock the mutex before accessing the shared resources
        pthread_mutex_lock(&mutex);

        printf("Thread1 %d writing into shared data. \n", thread_id);

        // set new value into shared_data
        shared_data = rand_r(&seed);
        // set condition to thrue, indicating that there is data to read
        condition = true;
        printf("Thread1 -> Current value: %d\n", shared_data);

        // unlock the mutex after accessing the shared resources
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

// function to be executed by thread2
void* reading_data(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 1; i <= 10; i++) {

        // checks if there is new data to read
        while (!condition) {}

        // lock the mutex before accessing the shared resources
        pthread_mutex_lock(&mutex);

        // reading data
        printf("Thread2 %d reading from shared data.\n", thread_id);
        printf("Thread2 -> Current value: %d\n", shared_data);
        // set condition to false, indicating that the data has been read
        condition = false;

        // unlock the mutex after accessing the shared resources
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t thread1, thread2;
    int id1 = 1, id2 = 2;

    // initialize the mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    // create threads
    if (pthread_create(&thread1, NULL, writing_data, &id1) !=0 || pthread_create(&thread2, NULL, reading_data, &id2) != 0) {
        perror("Thread creationg failed");
        return 1;
    }

    // wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}