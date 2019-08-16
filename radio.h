#ifndef _RADIO_H_
#define _RADIO_H_

#include "figura.h"
#include "texto.h"

typedef void * radio;


radio criaRadio(char* id, float x, float y, char *cor1, char *cor2, char *espessura);

void apagaRadio(radio h);

figura getRadioCirc(radio h);

texto getRadioText(radio h);

char *getRadioId(radio h);

#endif
