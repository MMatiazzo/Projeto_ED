#ifndef __SEG_H_
#define __SEG_H_

#include <stdio.h>
#include <string.h>

typedef void * segmento;


segmento criaSegmento(float x1, float y1, float x2, float y2);

float segGetX1(segmento s);

float segGetY1(segmento s);

float segGetX2(segmento s);

float segGetY2(segmento s);

int comparatorMuro(void *s1, void *s2);

int comparatorMuroAux(void *s1, void *s2);

void apagaSegmento(segmento seg);

void printaSvgMuroDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam);

#endif
