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
#define MAQ3 3

// TODO: Colocar nomes que fazem sentido
#define MINIMO_CHAPAS 10
#define EFICIENCIA_CONVERSAO_CHAPA_LATA 200
#define MINIMO_LATAS_BASICAS 50

#define TRUE 1

int chapasAluminio = CHAPAS;
int latasBasicas = 0;
int latasPintadas = 0;

sem_t mutexChapas;
sem_t mutexLatasBasicas;
sem_t mutexLatasPintadas;


// Thread máquina de criar chapas de alumínio
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

// Thread máquina de transfomar chapa em lata básica
void* maqFazerLatinha(void* id) {
    int i = *((int*)id);
    while (TRUE) {
        sem_wait(&mutexChapas);
        sem_wait(&mutexLatasBasicas);
            if (chapasAluminio >= MINIMO_CHAPAS) {
                chapasAluminio -= MINIMO_CHAPAS;
                latasBasicas += EFICIENCIA_CONVERSAO_CHAPA_LATA;
                printf("MaqFazerLatinha %d, Quantidade de latas: %d\n", i, latasBasicas);
            }
        sem_post(&mutexLatasBasicas);
        sem_post(&mutexChapas);
        sleep(1);
    }
}

void* maqPintarLatinha(void* id) {
    int i = *((int*)id);
    while(TRUE) {
        sem_wait(&mutexLatasBasicas);
        sem_wait(&mutexLatasPintadas);

        if (latasBasicas >= MINIMO_LATAS_BASICAS) {
            latasBasicas -= MINIMO_LATAS_BASICAS;
            latasPintadas += MINIMO_LATAS_BASICAS;
            printf("Máquina de pintar %d, Quantidade de latas pintadas: %d\n", i, latasPintadas);
        }

        sem_post(&mutexLatasPintadas);
        sem_post(&mutexLatasBasicas);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {

    int* id;

    // TODO: Lembrar de criar as threads
    pthread_t maq1[MAQ1];
    pthread_t maq2[MAQ2];
    pthread_t maq3[MAQ3];

    // TODO: Lembrar de iniciar os semáforos
    sem_init(&mutexChapas, 0, 1);
    sem_init(&mutexLatasBasicas, 0, 1);
    sem_init(&mutexLatasPintadas, 0, 1);

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

    return 0;

}
