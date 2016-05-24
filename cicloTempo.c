#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

static int cicloProdução;

void setParams(char* arg) {
    cicloProdução = setArguments(arg);
}

// TODO: Criar funcao para verificar o valor do ciclo, que foi recebido como argumento do make
int setArguments(char* arg) {
    int valorCiclo = atoi(arg);
    if (valorCiclo == 0) return 0;
    if (arg == NULL) return 24; // Ciclo default de 24h
    return valorCiclo;
}

// TODO: Criar funcao para startar o ciclo de produção


// TODO: