#ifndef _QUADRA_H_
#define _QUADRA_H_
#include "figura.h"

typedef void * quadra;

quadra criaQuadra(char * cep, float x, float y, float w, float h, char *cor1, char *cor2, char *espessura);

char * getCep(quadra q);

figura getQuadraRect(quadra q);

void apagaQuadra(quadra q);

#endif
