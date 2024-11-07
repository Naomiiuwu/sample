#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

int *buffer;
int buffer_size;
int in = 0, out = 0;
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void print_buffer() {
    printf("Buffer: [ ");
    for (int i = 0; i < buffer_size; i++) {
        if (i == in && i == out) {
            printf("I&O ");
        } else if (i == in) {
            printf("I ");
        } else if (i == out) {
            printf("O ");
        } else if (i < in && i >= out) {
            printf("%d ", buffer[i]);
        } else if (in < out && (i >= out || i < in)) {
            printf("%d ", buffer[i]);
        } else {
            printf("E ");
        }
    }
    printf("]\n");
}

void *producer(void *arg) {
    int item_count = *((int *)arg);
    int item;
    for (int i = 0; i < item_count; i++) {
        item = rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        in = (in + 1) % buffer_size;
        printf("Producer produced: %d\n", item);
        print_buffer();
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item_count = *((int *)arg);
    int item;
    for (int i = 0; i < item_count; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buffer[out];
        buffer[out] = 0;
        out = (out + 1) % buffer_size;
        printf("Consumer consumed: %d\n", item);
        print_buffer();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;
    int produce_count, consume_count;

    printf("Enter the buffer size: ");
    scanf("%d", &buffer_size);
    printf("Enter the number of items to produce: ");
    scanf("%d", &produce_count);
    printf("Enter the number of items to consume: ");
    scanf("%d", &consume_count);

    buffer = (int *)malloc(buffer_size * sizeof(int));
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = 0;
    }

    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod_thread, NULL, producer, &produce_count);
    pthread_create(&cons_thread, NULL, consumer, &consume_count);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    free(buffer);
    return 0;
}

a