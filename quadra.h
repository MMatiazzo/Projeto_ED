#ifndef _QUADRA_H_
#define _QUADRA_H_
#include "figura.h"

#include <stdio.h>

typedef void * quadra;

quadra criaQuadra(char * cep, float x, float y, float w, float h, char *cor1, char *cor2, char *espessura);

char * getCep(quadra q);

figura getQuadraRect(quadra q);

int comparatorQuadra(quadra q1, quadra q2);

void apagaQuadra(quadra q);

char *quadraToString(quadra e);

void printaSvgQuadDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam);

int verificaDentroQuadra(void* data,void** vetPontos,int tam,void* T,void *arquivoSVG,void* node);

#endif
