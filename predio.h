#ifndef _PRED_H_
#define _PRED_H_
#include "figura.h"
#include <stdio.h>


typedef void * predio;


predio criaPredio(char *cep, char *face, float num, float f, float p, float mrg, figura rectQuadra);

figura getPredioRect(predio p);

int comparatorPredio(predio p1, predio p2);

void apagaPredio(predio p);

char * getPredioFace(predio p);

char *getPredioCep(predio p);

float getPredioFrente(predio p);

float getPredioMargem(predio p);

float getPredioNum(predio p);

float getPredioProfundidade(predio p);

void printaSvgPredioDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam);

int verificaDentroPredio(void* data,void** vetPontos,int tam,void* T,void *arquivoSVG,void* node);

#endif
