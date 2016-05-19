//
// Created by Rafael Dias Silveira,
// matrícula 14/0030433, on 17/05/16.
//

#include <pthread.h>
#include <stdio.h>
#include <zconf.h>

#define CHAPAS 100
#define MAQ1 5
#define MAQ2 1
#define Y 10
#define X 200
#define TRUE 1

int chapasAluminio = CHAPAS;
int latasBasicas = 0;

pthread_mutex_t lockChapas = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockLatasBasicas = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t condFazerLatinhas = PTHREAD_COND_INITIALIZER;
pthread_cond_t condChapaAluminio = PTHREAD_COND_INITIALIZER;

void* maqChapaAlumunio() {
    while (TRUE) {
        pthread_mutex_lock(&lockChapas);
            if (chapasAluminio <= Y) {
                chapasAluminio += 10;
                printf("Quantidade de chapas de aluminio: %d\n", chapasAluminio);
                sleep(2);
            } else {
                pthread_cond_broadcast(&condFazerLatinhas);
                if (chapasAluminio >= CHAPAS) {
                    pthread_cond_wait(&condChapaAluminio, &lockChapas);
                }
            }
        pthread_mutex_unlock(&lockChapas);
    }
}

void* maqFazerLatinha() {
    while (TRUE) {
        pthread_mutex_lock(&lockChapas);
        pthread_mutex_lock(&lockLatasBasicas);
            if (chapasAluminio >= Y) {
                chapasAluminio -= Y;
                latasBasicas += X;
                printf("Quantidade de latas: %d\n", latasBasicas);
             //   sleep(2);
            } else {
                pthread_cond_broadcast(&condChapaAluminio);
                pthread_mutex_unlock(&lockLatasBasicas);
                pthread_cond_wait(&condFazerLatinhas, &lockChapas);
            }
        pthread_mutex_unlock(&lockLatasBasicas);
        pthread_mutex_unlock(&lockChapas);
    }
}

int main() {

    pthread_t maq1[MAQ1];
    pthread_t maq2[MAQ2];

    for (int i = 0; i < MAQ1; i++) {
        pthread_create(&maq1[i], NULL, maqChapaAlumunio, NULL);
    }
    for (int i = 0; i < MAQ2; i++) {
        pthread_create(&maq2[i], NULL, maqFazerLatinha(), NULL);
    }

    for (int i = 0; i < MAQ1; i++) {
        pthread_join(maq1[i], NULL);
    }
    for (int i = 0; i < MAQ2; i++) {
        pthread_join(maq2[i], NULL);
    }

    return 0;

}