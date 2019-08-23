#ifndef _PRED_H_
#define _PRED_H_
#include "figura.h"


typedef void * predio;


predio criaPredio(char *cep, char *face, float num, float f, float p, float mrg, figura rectQuadra);

figura getPredioRect(predio p);

void apagaPredio(predio p);

char * getPredioFace(predio p);

char *getPredioCep(predio p);


#endif
