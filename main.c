//
// Created by Rafael Dias Silveira,
// matrícula 14/0030433, on 17/05/16.
//

#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <semaphore.h>

#define CHAPAS 100
#define MAQ1 3
#define MAQ2 5
#define Y 10
#define X 200
#define TRUE 1

int chapasAluminio = CHAPAS;
int latasBasicas = 0;


sem_t mutexChapas;

pthread_mutex_t lockChapas       = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockLatasBasicas = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condFazerLatinhas = PTHREAD_COND_INITIALIZER;
pthread_cond_t condChapaAluminio = PTHREAD_COND_INITIALIZER;

void* maqChapaAlumunio(void* id) {
    int i = *((int*)id);
    while (TRUE) {
        sem_wait(&mutexChapas);
            if (chapasAluminio < CHAPAS) {
                chapasAluminio += 10;
                printf("MaqChapaAluminio %d, Quantidade de chapas de aluminio: %d\n", i,
                       chapasAluminio);
                sleep(1);
            }
        sem_post(&mutexChapas);
        sleep(1);
    }
}

void* maqFazerLatinha(void* id) {
    int i = *((int*)id);
    while (TRUE) {
        sem_wait(&mutexChapas);
            if (chapasAluminio >= Y) {
                chapasAluminio -= Y;
                latasBasicas += X;
                printf("MaqFazerLatinha %d, Quantidade de latas: %d\n", i, latasBasicas);
            }
        sem_post(&mutexChapas);
        sleep(1);
    }
}

int main() {

    int* id;

    pthread_t maq1[MAQ1];
    pthread_t maq2[MAQ2];

    sem_init(&mutexChapas, 0, 1);

    // Loops para a criação da threads
    for (int i = 0; i < MAQ1; i++) {
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&maq1[i], NULL, maqChapaAlumunio, (void*)id);
    }
    for (int i = 0; i < MAQ2; i++) {
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&maq2[i], NULL, maqFazerLatinha, (void*)id);
    }

    // Loop de join das threads
    for (int i = 0; i < MAQ1; i++) {
        pthread_join(maq1[i], NULL);
    }
    for (int i = 0; i < MAQ2; i++) {
        pthread_join(maq2[i], NULL);
    }

    return 0;

}