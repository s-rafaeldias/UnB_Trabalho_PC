#include <pthread.h>
#include <stdlib.h>
#include "cicloTempo.h"

static int cicloProducao = 0;
static double countTime = 0;
static double diffTime = 0;
static time_t tempoInicio;
static time_t tempoFim;

int getCicloProducao() {
    return cicloProducao;
}

int setValues(char* arg) {
    int valorCiclo = atoi(arg);
    if (valorCiclo == 0) return 0;
    if (arg == NULL) return 24; // Ciclo default de 24h
    return valorCiclo;
}

void setCicloProducao(int argc, char* argv[]) {
    if (argc == 2) cicloProducao = setValues(argv[1]);
    else if (argc == 1) cicloProducao = setValues(NULL);
}

void startCiclo() {
    tempoInicio = time(NULL);
}

// TODO: Função para calcular a hora do ciclo
int calculaHora() {
    tempoFim = time(NULL);
    diffTime = difftime(tempoFim, tempoInicio);
    if (diffTime == countTime) {
        countTime++;
        return 1;
    }
    return 0;
}
