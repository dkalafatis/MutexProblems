#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0; // Shared buffer
pthread_mutex_t mutex; // Mutex to control access to buffer
pthread_cond_t cond; // Condition variable
int data_available = 0; // Flag to indicate data availability

void *writer(void *arg) {
    int i;
    for(i = 1; i <= 10; i++) {
        // Lock the mutex before writing to the buffer
        pthread_mutex_lock(&mutex);
        while (data_available) {
            // Wait until the reader reads the data
            pthread_cond_wait(&cond, &mutex);
        }
        buffer = i;
        printf("Writer wrote: %d\n", buffer);
        data_available = 1; // Data is now available
        // Signal the reader that data is available
        pthread_cond_signal(&cond);
        // Unlock the mutex after writing
        pthread_mutex_unlock(&mutex);
        sleep(1); // Sleep for a while to simulate time taken to produce data
    }
    pthread_exit(NULL);
}

void *reader(void *arg) {
    int data;
    int i;
    for(i = 1; i <= 10; i++) {
        // Lock the mutex before reading from the buffer
        pthread_mutex_lock(&mutex);
        while (!data_available) {
            // Wait until the writer writes data
            pthread_cond_wait(&cond, &mutex);
        }
        data = buffer;
        printf("Reader read: %d\n", data);
        data_available = 0; // Data has been consumed
        // Signal the writer that data has been consumed
        pthread_cond_signal(&cond);
        // Unlock the mutex after reading
        pthread_mutex_unlock(&mutex);
        sleep(1); // Sleep for a while to simulate time taken to consume data
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t writer_thread, reader_thread;

    // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create the writer and reader threads
    pthread_create(&writer_thread, NULL, writer, NULL);
    pthread_create(&reader_thread, NULL, reader, NULL);

    // Wait for the threads to finish
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
