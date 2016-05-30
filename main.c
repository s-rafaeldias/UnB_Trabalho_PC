//
// Created by Rafael Dias Silveira,
// matrícula 14/0030433, on 17/05/16.
//

#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <semaphore.h>
#include "cicloTempo.h"

#define CHAPAS 100

#define MAQ1 3
#define MAQ2 5
#define MAQ3 3

// TODO: Colocar nomes que fazem sentido
#define MINIMO_CHAPAS 10
#define EFICIENCIA_CONVERSAO_CHAPA_LATA 200
#define MINIMO_LATAS_BASICAS 50

#define TRUE 1

int chapasAluminio = CHAPAS;
int latasBasicas   = 0;
int latasPintadas  = 0;

int ciclo = 0;
int flag = 0;

sem_t mutexChapas;
sem_t mutexLatasBasicas;
sem_t mutexLatasPintadas;
sem_t mutexFlag;

pthread_cond_t condChapas        = PTHREAD_COND_INITIALIZER;
pthread_cond_t condLatasBasicas  = PTHREAD_COND_INITIALIZER;
pthread_cond_t condLatasPintadas = PTHREAD_COND_INITIALIZER;

pthread_mutex_t pqSim  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pqSim2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pqSim3 = PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t barreira;

// Thread máquina de criar chapas de alumínio
void* maqChapaAlumunio(void* id) {
    int i = *((int*)id);
    while (TRUE) {
        sem_wait(&mutexChapas);
        if (chapasAluminio < CHAPAS) {
            chapasAluminio += 10;
            sleep(1);
        }
        sem_post(&mutexChapas);
        // Sicronização para utilização do log

        // Quando o ciclo de trabalho estiver completo, encerra a thread
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// Thread máquina de transfomar chapa em lata básica
void* maqFazerLatinha(void* id) {
    int i = *((int*)id);
    while (TRUE) {
        sem_wait(&mutexChapas);
        sem_wait(&mutexLatasBasicas);
        if (chapasAluminio >= MINIMO_CHAPAS) {
            chapasAluminio -= MINIMO_CHAPAS;
            latasBasicas += EFICIENCIA_CONVERSAO_CHAPA_LATA;
        }
        sem_post(&mutexLatasBasicas);
        sem_post(&mutexChapas);
        // Sicronização para utilização do log

        // Quando o ciclo de trabalho estiver completo, encerra a thread
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// Thread máquina de pintar latas
void* maqPintarLatinha(void* id) {
    int i = *((int*)id);
    while(TRUE) {
        sem_wait(&mutexLatasBasicas);
        sem_wait(&mutexLatasPintadas);
        if (latasBasicas >= MINIMO_LATAS_BASICAS) {
            latasBasicas -= MINIMO_LATAS_BASICAS;
            latasPintadas += MINIMO_LATAS_BASICAS;
        }
        sem_post(&mutexLatasPintadas);
        sem_post(&mutexLatasBasicas);
        // Sicronização para utilização do log

        // Quando o ciclo de trabalho estiver completo, encerra a thread
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// Thread que verifica o status do ciclo de operação. A cada hora,
void* cicloOperacao() {
    while (TRUE) {
        if (calculaHora()) {
            ciclo++;
            printf("Flag: %d\n", flag);
        }
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// TODO: mudar esse nome, pq né
void* syncPorraToda() {
    while (TRUE) {
        if (flag == MAQ1+MAQ2+MAQ3) {
            printf("Ciclo: %d\n", ciclo);
            flag = 0;
            pthread_cond_broadcast(&condChapas);
            pthread_cond_broadcast(&condLatasBasicas);
            pthread_cond_broadcast(&condLatasPintadas);
        }
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

int main(int argc, char* argv[]) {

    int* id;

    // TODO: Lembrar de criar as threads
    pthread_t maq1[MAQ1];
    pthread_t maq2[MAQ2];
    pthread_t maq3[MAQ3];
    pthread_t sync_t;


    pthread_t status;

    // TODO: Lembrar de iniciar os semáforos
    sem_init(&mutexChapas, 0, 1);
    sem_init(&mutexLatasBasicas, 0, 1);
    sem_init(&mutexLatasPintadas, 0, 1);
    sem_init(&mutexFlag, 0, 1);

    // TODO: Barreira de ciclo de funcionamento
    pthread_barrier_init(&barreira, NULL, 11);

    setCicloProducao(argc, argv);

    startCiclo();

    // TODO: Lembrar de criar o loop de criação de threads
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
    for (int i = 0; i < MAQ3; i++) {
        id = (int*) malloc(sizeof(int));
        *id = i;
        pthread_create(&maq3[i], NULL, maqPintarLatinha, (void*)id);
    }

    pthread_create(&status, NULL, cicloOperacao, NULL);
    pthread_create(&sync_t, NULL, syncPorraToda, NULL);

    // TODO: Dar join nas threads
    // Loop de join das threads
    for (int i = 0; i < MAQ1; i++) {
        pthread_join(maq1[i], NULL);
    }
    for (int i = 0; i < MAQ2; i++) {
        pthread_join(maq2[i], NULL);
    }
    for (int i = 0; i < MAQ3; i++) {
        pthread_join(maq3[i], NULL);
    }
    pthread_join(status, NULL);
    pthread_join(sync_t, NULL);

    printf("Chapas Aluminio: %d\nLatas Basicas: %d\nLatas Pintadas: %d\n", chapasAluminio, latasBasicas, latasPintadas);
    printf("Flag %d\n", flag);

    return 0;

}
