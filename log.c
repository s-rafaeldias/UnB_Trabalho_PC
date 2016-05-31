//
// Created by rafael on 30/05/16.
//

#include <stdio.h>
#include "log.h"

static FILE* saida1;
static FILE* saida2;
static FILE* saida3;

static void preencheBase(int hora, FILE* f) {

    fprintf(f, "Hora:,");

    for (int i = 0; i < hora; i++) {
        fprintf(f, "%d,", i+1);
    }

    fprintf(f, "\n");
    fprintf(f, "Prod:,");
}


void baseLog(int hora) {

    saida1 = fopen("log1.csv", "w+");
    saida2 = fopen("log2.csv", "w+");
    saida3 = fopen("log3.csv", "w+");

    preencheBase(hora, saida1);
    preencheBase(hora, saida2);
    preencheBase(hora, saida3);

}

void printaLog(int prod, int i) {



}

void encerraTudo() {
    fclose(saida1);
    fclose(saida2);
    fclose(saida3);
}