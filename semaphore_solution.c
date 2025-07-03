#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// Define a binary semaphore using POSIX semaphores
typedef sem_t BinarySemaphore;

// Wrapper to initialize binary semaphore
void binary_semaphore_init(BinarySemaphore *sem, int value) {
    sem_init(sem, 0, value);
}

// Wrapper to wait (decrement) semaphore
void binary_semaphore_wait(BinarySemaphore *sem) {
    sem_wait(sem);
}

// Wrapper to signal (increment) semaphore
void binary_semaphore_signal(BinarySemaphore *sem) {
    sem_post(sem);
}

// Try to decrement without blocking
int binary_semaphore_try_wait(BinarySemaphore *sem) {
    return sem_trywait(sem);
}

// Semaphores representing ingredients and control
BinarySemaphore tobacco, paper, match, next_round;

// Agent thread function
// Randomly selects one ingredient to exclude and gives the other two
void* agent(void* arg) {
    while (1) {
        binary_semaphore_wait(&next_round); // Wait for smoker to finish

        printf("Agent : Producing ");

        int num = rand() % 3; // Randomly exclude one ingredient

        switch (num) {
            case 0:
                printf("Paper and Match\n");
                binary_semaphore_signal(&paper);
                binary_semaphore_signal(&match);
                break;
            case 1:
                printf("Match and Tobacco\n");
                binary_semaphore_signal(&match);
                binary_semaphore_signal(&tobacco);
                break;
            case 2:
                printf("Tobacco and Paper\n");
                binary_semaphore_signal(&tobacco);
                binary_semaphore_signal(&paper);
                break;
        }
    }
    return NULL;
}

// Smoker with tobacco waits for paper and match
void* smoker_with_tobacco(void* arg) {
    while (1) {
        binary_semaphore_wait(&paper); // Wait for paper

        if (binary_semaphore_try_wait(&match) == 0) { // Try to get match
            printf("Smoker having tobacco : Smoking...\n");
            sleep(1);
            printf("Smoker having tobacco : Finished smoking ! \n\n");

            binary_semaphore_signal(&next_round); // Signal agent to continue
        } else {
            binary_semaphore_signal(&paper); // Release paper if match not available
        }
    }
    return NULL;
}

// Smoker with paper waits for match and tobacco
void* smoker_with_paper(void* arg) {
    while (1) {
        binary_semaphore_wait(&match); // Wait for match

        if (binary_semaphore_try_wait(&tobacco) == 0) { // Try to get tobacco
            printf("Smoker having paper : Smoking...\n");
            sleep(1);
            printf("Smoker having paper : Finished smoking ! \n\n");

            binary_semaphore_signal(&next_round);
        } else {
            binary_semaphore_signal(&match);
        }
    }
    return NULL;
}

// Smoker with match waits for tobacco and paper
void* smoker_with_match(void* arg) {
    while (1) {
        binary_semaphore_wait(&tobacco); // Wait for tobacco

        if (binary_semaphore_try_wait(&paper) == 0) { // Try to get paper
            printf("Smoker having match : Smoking...\n");
            sleep(1);
            printf("Smoker having match : Finished smoking ! \n\n");

            binary_semaphore_signal(&next_round);
        } else {
            binary_semaphore_signal(&tobacco);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Initialize random seed

    // Initialize binary semaphores
    binary_semaphore_init(&tobacco, 0);
    binary_semaphore_init(&paper, 0);
    binary_semaphore_init(&match, 0);
    binary_semaphore_init(&next_round, 1); // Agent starts first

    pthread_t smoker_threads[3], agent_thread;

    // Create smoker threads
    pthread_create(&smoker_threads[0], NULL, smoker_with_tobacco, NULL);
    pthread_create(&smoker_threads[1], NULL, smoker_with_paper, NULL);
    pthread_create(&smoker_threads[2], NULL, smoker_with_match, NULL);
    printf("Smoker threads created\n");

    sleep(1); // Optional: give time for smokers to initialize

    // Create agent thread
    pthread_create(&agent_thread, NULL, agent, NULL);
    printf("Agent thread created\n");

    sleep(1); // Optional

    // Join all threads (infinite loop, won't actually return unless externally stopped)
    pthread_join(smoker_threads[0], NULL);
    pthread_join(smoker_threads[1], NULL);
    pthread_join(smoker_threads[2], NULL);
    pthread_join(agent_thread, NULL);

    printf("All threads joined\n");
    return 0;
}