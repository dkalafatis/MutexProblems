#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define WAREHOUSE_SIZE 1

int warehouse;            // The data item in the warehouse
int warehouse_full = 0;   // 0 if empty, 1 if full

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Function to add an item to the warehouse
void add(int item) {
    pthread_mutex_lock(&mutex);
    while (warehouse_full) {
        pthread_cond_wait(&not_full, &mutex);
    }
    // Add item to warehouse
    warehouse = item;
    warehouse_full = 1;
    printf("Produced item %d\n", item);
    // Signal that warehouse is not empty
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
}

// Function to remove an item from the warehouse
int remove_item() {
    int item;
    pthread_mutex_lock(&mutex);
    while (!warehouse_full) {
        pthread_cond_wait(&not_empty, &mutex);
    }
    // Remove item from warehouse
    item = warehouse;
    warehouse_full = 0;
    printf("Consumed item %d\n", item);
    // Signal that warehouse is not full
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);
    return item;
}

// Producer thread function
void* producer(void* arg) {
    int item = 0;
    while (1) {
        // Produce an item
        item++;
        add(item);
        sleep(1); // Sleep for one unit time
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    while (1) {
        int item = remove_item();
        // Process the item
        sleep(1); // Sleep for one unit time
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Join threads (this will actually never happen in this infinite loop)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
