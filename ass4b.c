#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

sem_t rw_mutex;
sem_t mutex;
int read_count = 0;

void* reader(void* arg) {
    int reader_id = *((int*)arg);
    int iterations = *((int*)(arg + sizeof(int)));
    for (int i = 0; i < iterations; i++) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex);

        printf("Reader %d is reading\n", reader_id);
        fflush(stdout);
        sleep(1);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex);
        }
        sem_post(&mutex);
        sleep(1);
    }
    printf("Reader %d has finished reading\n", reader_id);
    fflush(stdout);
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);
    int iterations = *((int*)(arg + sizeof(int)));
    for (int i = 0; i < iterations; i++) {
        sem_wait(&rw_mutex);

        printf("Writer %d is writing\n", writer_id);
        fflush(stdout);
        sleep(1);

        sem_post(&rw_mutex);
        sleep(2);
    }
    printf("Writer %d has finished writing\n", writer_id);
    fflush(stdout);
    return NULL;
}

int main() {
    int num_readers, num_writers;
    printf("Enter the number of readers: ");
    fflush(stdout);
    scanf("%d", &num_readers);
    printf("Enter the number of writers: ");
    fflush(stdout);
    scanf("%d", &num_writers);

    pthread_t readers[num_readers], writers[num_writers];
    int reader_ids[num_readers], writer_ids[num_writers];
    int read_iterations[num_readers], write_iterations[num_writers];

    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1;
        printf("Enter the number of read operations for Reader %d: ", reader_ids[i]);
        fflush(stdout);
        scanf("%d", &read_iterations[i]);
    }

    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;
        printf("Enter the number of write operations for Writer %d: ", writer_ids[i]);
        fflush(stdout);
        scanf("%d", &write_iterations[i]);
    }

    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    for (int i = 0; i < num_readers; i++) {
        int* args = malloc(2 * sizeof(int));
        args[0] = reader_ids[i];
        args[1] = read_iterations[i];
        pthread_create(&readers[i], NULL, reader, args);
    }

    for (int i = 0; i < num_writers; i++) {
        int* args = malloc(2 * sizeof(int));
        args[0] = writer_ids[i];
        args[1] = write_iterations[i];
        pthread_create(&writers[i], NULL, writer, args);
    }

    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    return 0;
}

