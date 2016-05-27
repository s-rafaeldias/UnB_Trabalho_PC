#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

static int cicloProdução = 0;
static time_t tempoInicio;
static time_t tempoFim;
static double countTime = 0;
static double diffTime = 0;

void setCicloProducao(char* arg) {
    cicloProdução = setValues(arg);
}

int setValues(char* arg) {
    int valorCiclo = atoi(arg);
    if (valorCiclo == 0) return 0;
    if (arg == NULL) return 24; // Ciclo default de 24h
    return valorCiclo;
}

void startCiclo() {
    tempoInicio = time();
}

// TODO: Função para calcular a hora do ciclo
int calculaHora() {
    tempoFim = time();
    diffTime = difftime(tempoFim, tempoInicio);
    if (diffTime == countTime) {
        countTime++;
        return 1;
    }
    return 0;
}
