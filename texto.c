#include "texto.h"
#include <string.h>
#include <stdlib.h>

struct texto{
  float x,y;
  char *texto;
};



texto criaTexto(float x, float y, char* entrada){
  struct texto *this;
  this = (struct texto*) malloc(sizeof(struct texto));
  this->x = x;
  this->y = y;
  this->texto = malloc(sizeof(char) * (strlen(entrada) + 1));
  strcpy(this->texto, entrada);
  return (void *) this;
}

void apagaTexto(texto text){
  struct texto *this;
  this = (struct texto *) text;
  free(this->texto);
  free(this);
}


float getXtexto(texto text){
  struct texto *this;
  this = (struct texto *) text;
  return this->x;
}


float getYtexto(texto text){
  struct texto *this;
  this = (struct texto *) text;
  return this->y;
}

void setXtexto(texto text, float x){
  struct texto *this;
  this = (struct texto *) text;
  this->x = x;
}

void setYtexto(texto text, float y){
  struct texto *this;
  this = (struct texto *) text;
  this->y = y;
}


char *getTexto(texto text){
  struct texto *this;
  this = (struct texto *) text;
  return this->texto;
}
