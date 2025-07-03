#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 10

int t = 0;
pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t ingredients[N];
sem_t targets[1 << N];
sem_t next_round;

void* agent(void* arg) {
    while (1) {
        sem_wait(&next_round);
        int notUsed = rand() % N;
        printf("Agent: Producing items: ");
        for (int i = 0; i < N; i++) {
            if (i != notUsed) {
                printf("%d ", i);
                sem_post(&ingredients[i]);
            }
        }
        printf("\n");
    }
    return NULL;
}

void* f(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&ingredients[id]);
        pthread_mutex_lock(&t_mutex);
        t += (1 << id);
        sem_post(&targets[t]);
        pthread_mutex_unlock(&t_mutex);
    }
    return NULL;
}

void* smoker(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&targets[(1 << N) - 1 - (1 << id)]);
        printf("Smoker %d: Smoking...\n", id);
        sleep(1);
        printf("Smoker %d: Finished smoking\n\n", id);
        pthread_mutex_lock(&t_mutex);
        t = 0;
        pthread_mutex_unlock(&t_mutex);
        sem_post(&next_round);
    }
    return NULL;
}

int main() {
    pthread_t agent_thread, f_threads[N], smoker_threads[N];
    int smoker_ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&ingredients[i], 0, 0);
        sem_init(&targets[i], 0, 0);
    }
    for (int i = N; i < (1 << N); i++) {
        sem_init(&targets[i], 0, 0);
    }
    sem_init(&next_round, 0, 1);

    for (int i = 0; i < N; i++) {
        smoker_ids[i] = i;
        pthread_create(&f_threads[i], NULL, f, &smoker_ids[i]);
        pthread_create(&smoker_threads[i], NULL, smoker, &smoker_ids[i]);
    }

    pthread_create(&agent_thread, NULL, agent, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(f_threads[i], NULL);
        pthread_join(smoker_threads[i], NULL);
    }
    pthread_join(agent_thread, NULL);
    return 0;
}