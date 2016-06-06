//
// Created by Rafael Dias Silveira,
// matrícula 14/0030433, on 17/05/16.
//

#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "cicloTempo.h"
#include "log.h"

#define CHAPAS 100 // Default da quantidade de chapas iniciais

// Definicao da quantidade de cada tipo de thread
#define MAQ1 4  // Quantidade de Threads da máquina de criação de chapas de alumínio
#define MAQ2 7  // Quantidade de Threads da máquina de criação de latas básicas
#define MAQ3 10 // Quantidade de Threads da máquina de pintar latas
#define MAQ4 5  // Quantidade de Threads de caminhões de transporte

#define MINIMO_CHAPAS 20
#define EFICIENCIA_CONVERSAO_CHAPA_LATA 200
#define MINIMO_LATAS_BASICAS 50
#define MINIMO_LATAS_TRANSPORTE 500
#define ECONOMIA_ENERGIA 70

#define TRUE 1

int chapasAluminio = CHAPAS;
int latasBasicas   = 0;
int latasPintadas  = 0;

// Variáveis para armazenamento da produção de cada tipo de máquina e do transporte
int prodMaq1  = 0;
int prodMaq2  = 0;
int prodMaq3  = 0;
int prodTrans = 0;

// Variável para controle de ciclo de produção
int ciclo = 0;

pthread_mutex_t mutexChapas = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condChapas   = PTHREAD_COND_INITIALIZER;

// Semáforos utilizados como locks
// sem_t mutexChapas;        // Lock para chapasAluminio
sem_t mutexLatasBasicas;  // Lock para latasBasicas
sem_t mutexLatasPintadas; // Lock para latasPintadas

// Semáforos utilizados como locks
sem_t mutexProdMaq1;  // Lock para produção do ciclo de prodMaq1
sem_t mutexProdMaq2;  // Lock para produção do ciclo de prodMaq2
sem_t mutexProdMaq3;  // Lock para produção do ciclo de prodMaq3
sem_t mutexProdTrans; // Lock para produção do ciclo de prodTrans

// Barreira para o transporte e distribuição de latas
pthread_barrier_t barreiraTransporte;

// Thread máquina de criar chapas de alumínio
void* maqChapaAlumunio(void* id) {

    int i = *((int*)id);

    while (TRUE) {
        pthread_mutex_lock(&mutexChapas);
        if (chapasAluminio < CHAPAS) {
            chapasAluminio += 10;

            sem_wait(&mutexProdMaq1);
                prodMaq1 += 10;
            sem_post(&mutexProdMaq1);
        }
        if (chapasAluminio >= ECONOMIA_ENERGIA) {
            pthread_cond_wait(&condChapas, &mutexChapas);
        }
        pthread_mutex_unlock(&mutexChapas);

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
        pthread_mutex_lock(&mutexChapas);
        sem_wait(&mutexLatasBasicas);
        if (chapasAluminio <= MINIMO_CHAPAS) {
            pthread_cond_broadcast(&condChapas);
        }
        if (chapasAluminio >= MINIMO_CHAPAS) {
            chapasAluminio -= MINIMO_CHAPAS;
            latasBasicas += EFICIENCIA_CONVERSAO_CHAPA_LATA;

            sem_wait(&mutexProdMaq2);
                prodMaq2 += EFICIENCIA_CONVERSAO_CHAPA_LATA;
            sem_post(&mutexProdMaq2);

        }
        sem_post(&mutexLatasBasicas);
        pthread_mutex_unlock(&mutexChapas);

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
        sem_wait(&mutexLatasPintadas);
        sem_wait(&mutexLatasBasicas);
        if (latasBasicas >= MINIMO_LATAS_BASICAS) {

            latasBasicas -= MINIMO_LATAS_BASICAS;
            latasPintadas += MINIMO_LATAS_BASICAS;

            sem_wait(&mutexProdMaq3);
                prodMaq3 += MINIMO_LATAS_BASICAS;
            sem_post(&mutexProdMaq3);
        }

        sem_post(&mutexLatasBasicas);
        sem_post(&mutexLatasPintadas);

        // Quando o ciclo de trabalho estiver completo, encerra a thread
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// Thread transporte e distribuição de latas
void* transporteLata() {

    while(TRUE) {
        sem_wait(&mutexLatasPintadas);
        if (latasPintadas >= MINIMO_LATAS_TRANSPORTE) {
            latasPintadas -= MINIMO_LATAS_TRANSPORTE;

            sem_wait(&mutexProdTrans);
                prodTrans += MINIMO_LATAS_TRANSPORTE;
            sem_post(&mutexProdTrans);
        }
        sem_post(&mutexLatasPintadas);

        pthread_barrier_wait(&barreiraTransporte);

        // Quando o ciclo de trabalho estiver completo, encerra a thread
        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}

// Thread que verifica o status do ciclo de operação
void* cicloOperacao() {

    while (TRUE) {
        if (calculaHora()) {
            ciclo++;

            sem_wait(&mutexProdMaq1);
            sem_wait(&mutexProdMaq2);
            sem_wait(&mutexProdMaq3);
            sem_wait(&mutexProdTrans);

                printaLog(prodMaq1, 0);
                printaLog(prodMaq2, 1);
                printaLog(prodMaq3, 2);
                printaLog(prodTrans, 3);

                prodMaq1  = 0;
                prodMaq2  = 0;
                prodMaq3  = 0;
                prodTrans = 0;

            sem_post(&mutexProdTrans);
            sem_post(&mutexProdMaq3);
            sem_post(&mutexProdMaq2);
            sem_post(&mutexProdMaq1);
        }

        if (ciclo == getCicloProducao()) {
            pthread_exit(0);
        }
    }
}


int main(int argc, char* argv[]) {

    int* id;

    pthread_t maq1[MAQ1];
    pthread_t maq2[MAQ2];
    pthread_t maq3[MAQ3];
    pthread_t transporte[MAQ4];

    pthread_t status;

    // sem_init(&mutexChapas, 0, 1);
    sem_init(&mutexLatasBasicas, 0, 1);
    sem_init(&mutexLatasPintadas, 0, 1);

    sem_init(&mutexProdMaq1, 0, 1);
    sem_init(&mutexProdMaq2, 0, 1);
    sem_init(&mutexProdMaq3, 0, 1);
    sem_init(&mutexProdTrans, 0, 1);

    pthread_barrier_init(&barreiraTransporte, NULL, MAQ4);

    setCicloProducao(argc, argv);

    startCiclo();

    baseLog(getCicloProducao());

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
    for (int i = 0; i < MAQ4; i++) {
        pthread_create(&transporte[i], NULL, transporteLata, NULL);
    }

    pthread_create(&status, NULL, cicloOperacao, NULL);


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
    for (int i = 0; i < MAQ4; i++) {
        pthread_join(transporte[i], NULL);
    }

    pthread_join(status, NULL);

    printaLog(prodMaq1, 0);
    printaLog(prodMaq2, 1);
    printaLog(prodMaq3, 2);
    printaLog(prodTrans, 3);

    encerraTudo();

    return 0;

}
