#ifndef _TEXTO_H_
#define _TEXTO_H_

typedef void * texto;

texto criaTexto(float x, float y, char* entrada);

float getXtexto(texto text);

float getYtexto(texto text);

char *getTexto(texto text);

void setXtexto(texto text, float x);

void setYtexto(texto text, float y);

void apagaTexto(texto text);





#endif
