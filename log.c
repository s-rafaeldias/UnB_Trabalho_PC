//
// Created by rafael on 30/05/16.
//

#include <stdio.h>
#include "log.h"

static FILE* saida1;
static FILE* saida2;
static FILE* saida3;
static FILE* saida4;

static void fechaFiles() {

}

static void abreFiles() {

}

static void preencheBase(int hora, FILE* f) {

    fprintf(f, "Hora:,");

    for (int i = 0; i <= hora; i++) {
        fprintf(f, "%d,", i);
    }

    fprintf(f, "\n");
    fprintf(f, "Prod:,");
}


void baseLog(int hora) {

    saida1 = fopen("log1.csv", "a+");
    saida2 = fopen("log2.csv", "a+");
    saida3 = fopen("log3.csv", "a+");
    saida4 = fopen("log4.csv", "a+");

    preencheBase(hora, saida1);
    preencheBase(hora, saida2);
    preencheBase(hora, saida3);
    preencheBase(hora, saida4);


}

void printaLog(int prod, int i) {


    switch (i) {
        case 0:
            fprintf(saida1, "%d,", prod);
            break;
        case 1:
            fprintf(saida2, "%d,", prod);
            break;
        case 2:
            fprintf(saida3, "%d,", prod);
            break;
        case 3:
            fprintf(saida4, "%d,", prod);
            break;
        default:
            break;
    }


}

void encerraTudo() {
    fclose(saida1);
    fclose(saida2);
    fclose(saida3);
    fclose(saida4);
}