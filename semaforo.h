#ifndef _SEMAFORO_H_
#define _SEMAFORO_H_

#include "figura.h"
#include "texto.h"

typedef void * semaforo;


semaforo criaSemaforo(char *id, float x, float y, char *cor1, char *cor2, char *espessura);

void apagaSemaforo(semaforo h);

figura getSemaforoCirc(semaforo h);

texto getSemaforoText(semaforo h);

char *getSemaforoId(semaforo h);

int semaforoComparator(semaforo s1, semaforo s2, float x, float y);
#endif
