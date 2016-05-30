//
// Created by rafael on 30/05/16.
//

#include <stdio.h>
#include <pthread.h>
#include "log.h"

void baseLog(int hora) {
    FILE* saida = fopen("log.csv", "w+");

    fprintf(saida, "Hora:,");
    for (int i = 0; i < hora; i++) {
        fprintf(saida, "%d,", i+1);
    }

    fprintf(saida, "\n");
    fprintf(saida, "Prod:,");

    fclose(saida);
}

void printaLog(int prod1, int prod2, int prod3) {

    pthread_t printStatus[3];

    for (int i = 0; i < 3; i++) {
        pthread_create(&printStatus[i], NULL, , NULL);
    }

}

