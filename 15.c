#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int var1, var2;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int threads_read = 0;

void* thread1_func(void* arg) {
    // Lock mutex1 to read var1
    pthread_mutex_lock(&mutex1);
    int value = var1;
    printf("Thread 1 read var1 = %d\n", value);
    pthread_mutex_unlock(&mutex1);

    // Signal to main process
    pthread_mutex_lock(&mutex);
    threads_read++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* thread2_func(void* arg) {
    // Lock mutex2 to read var2
    pthread_mutex_lock(&mutex2);
    int value = var2;
    printf("Thread 2 read var2 = %d\n", value);
    pthread_mutex_unlock(&mutex2);

    // Signal to main process
    pthread_mutex_lock(&mutex);
    threads_read++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // Initialize var1 and var2
    var1 = 10;
    var2 = 20;

    pthread_t thread1, thread2;

    // Create the threads
    pthread_create(&thread1, NULL, thread1_func, NULL);
    pthread_create(&thread2, NULL, thread2_func, NULL);

    // Wait until both threads have read the variables
    pthread_mutex_lock(&mutex);
    while (threads_read < 2) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Swap var1 and var2
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);

    int temp = var1;
    var1 = var2;
    var2 = temp;

    printf("Main thread swapped var1 and var2\n");
    printf("var1 = %d, var2 = %d\n", var1, var2);

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    // Join threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destroy mutexes and condition variable
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
