#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];
int buffer_index = 0;
int writer_done = 0;

pthread_mutex_t buffer_mutex;

void *writer_thread(void *arg) {
    const char *message = (const char *)arg;
    int i = 0;
    while (message[i] != '\0') {
        pthread_mutex_lock(&buffer_mutex);

        if (buffer_index < BUFFER_SIZE - 1) {
            buffer[buffer_index++] = message[i];
            buffer[buffer_index] = '\0';  // Null-terminate the string
            i++;
        } else {
            // Buffer is full
            pthread_mutex_unlock(&buffer_mutex);
            break;
        }

        pthread_mutex_unlock(&buffer_mutex);

        sleep(1); // Simulate writing delay
    }
    pthread_mutex_lock(&buffer_mutex);
    writer_done = 1;
    pthread_mutex_unlock(&buffer_mutex);
    return NULL;
}

void read_buffer() {
    pthread_mutex_lock(&buffer_mutex);

    printf("Buffer contains: %s\n", buffer);

    pthread_mutex_unlock(&buffer_mutex);
}

int main() {
    pthread_t tid;
    const char *message = "Hello, this is a test message.";

    // Initialize mutex
    pthread_mutex_init(&buffer_mutex, NULL);

    // Initialize buffer
    buffer[0] = '\0';

    // Create writer thread
    if (pthread_create(&tid, NULL, writer_thread, (void *)message)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Read from buffer periodically until writer is done
    while (1) {
        pthread_mutex_lock(&buffer_mutex);
        int done = writer_done;
        pthread_mutex_unlock(&buffer_mutex);

        read_buffer();

        if (done)
            break;

        sleep(1); // Sleep for 1 second
    }

    // Wait for the writer thread to finish
    pthread_join(tid, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&buffer_mutex);

    return 0;
}
