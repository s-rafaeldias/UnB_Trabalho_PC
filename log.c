//
// Created by rafael on 30/05/16.
//

#include <stdio.h>
#include "log.h"

static FILE* saida;

static void abreArquivo(int file);
void printLog(int file, int m[][48], int hora, int size);

////////////////////////////////////////////////////////////////////////////////////////////////////
void printLog(int file, int m[][48], int hora, int size) {

  abreArquivo(file);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < hora; j++) {
      fprintf(saida, "%d,", m[i][j]);
    } // for j
    fprintf(saida, "\n");
  } // for i

  fclose(saida);

}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void abreArquivo(int file) {

  switch (file) {
    case 1:
      saida = fopen("log1.csv", "w");
    break;

    case 2:
      saida = fopen("log2.csv", "w");
    break;

    case 3:
      saida = fopen("log3.csv", "w");
    break;

    case 4:
      saida = fopen("log4.csv", "w");
    break;
  }

}
////////////////////////////////////////////////////////////////////////////////////////////////////
