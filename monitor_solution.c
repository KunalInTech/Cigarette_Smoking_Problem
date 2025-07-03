#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 10

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t next_round = PTHREAD_COND_INITIALIZER;
pthread_cond_t smokers[N];
int current_smoker = -1;

void dropIngredients(int *ingredients, int len) {
    pthread_mutex_lock(&mtx);
    while (current_smoker != -1) {
        pthread_cond_wait(&next_round, &mtx);
    }

    printf("Agent: Producing items: ");
    for (int i = 0; i < len; ++i) {
        printf("%d ", ingredients[i]);
    }
    printf("\n");

    int missing = 0;
    for (int i = 0; i < N; ++i) missing ^= i;
    for (int i = 0; i < len; ++i) missing ^= ingredients[i];

    current_smoker = missing;
    pthread_cond_signal(&smokers[missing]);
    pthread_mutex_unlock(&mtx);
}

void pickupIngredients(int id) {
    pthread_mutex_lock(&mtx);
    while (current_smoker != id) {
        pthread_cond_wait(&smokers[id], &mtx);
    }
    pthread_mutex_unlock(&mtx);
}

void notifyForNextRound() {
    pthread_mutex_lock(&mtx);
    current_smoker = -1;
    pthread_cond_signal(&next_round);
    pthread_mutex_unlock(&mtx);
}

void* smoker(void* arg) {
    int id = *(int*)arg;
    while (1) {
        pickupIngredients(id);
        printf("Smoker %d: Smoking...\n", id);
        sleep(1);
        printf("Smoker %d: Finished smoking\n\n", id);
        notifyForNextRound();
    }
    return NULL;
}

void* agent(void* arg) {
    while (1) {
        int exclude = rand() % N;
        int items[N - 1];
        int idx = 0;
        for (int j = 0; j < N; ++j) {
            if (j != exclude) items[idx++] = j;
        }
        dropIngredients(items, N - 1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t smoker_threads[N], agent_thread;
    int smoker_ids[N];

    for (int i = 0; i < N; ++i) {
        pthread_cond_init(&smokers[i], NULL);
        smoker_ids[i] = i;
        pthread_create(&smoker_threads[i], NULL, smoker, &smoker_ids[i]);
    }

    printf("Smoker threads created\n");
    sleep(1);

    pthread_create(&agent_thread, NULL, agent, NULL);
    printf("Agent thread created\n\n");

    for (int i = 0; i < N; ++i) {
        pthread_join(smoker_threads[i], NULL);
    }
    pthread_join(agent_thread, NULL);
    return 0;
}