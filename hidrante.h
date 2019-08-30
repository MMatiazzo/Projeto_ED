#ifndef _HIDRANTE_H_
#define _HIDRANTE_H_

#include "texto.h"
#include "figura.h"

typedef void* hidrante;


hidrante criaHidrante(char *id, float x, float y, char *cor1, char *cor2, char *espessura);

figura getHidranteCirc(hidrante h);

texto getHidranteText(hidrante h);

char *getHidranteId(hidrante h);

void apagaHidrante(hidrante h);

int hidranteComparator(hidrante s1, hidrante s2, float x, float y);

#endif
