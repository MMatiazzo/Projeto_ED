#include "radio.h"
#include <stdlib.h>
#include <string.h>

struct radio{
  texto text;
  char *id;
  figura circ;
};


radio criaRadio(char *id, float x, float y, char *cor1, char *cor2, char *espessura){
  struct radio *this;
  this = (struct radio *) malloc(sizeof(struct radio));
  this->id = (char *)malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-3, 10.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "R");
  strcpy(this->id, id);
  return (radio) this;
}



figura getRadioCirc(radio h){
  struct radio *this;
  this = (struct radio *) h;
  return this->circ;
}


texto getRadioText(radio h){
  struct radio *this;
  this = (struct radio *) h;
  return this->text;
}


char *getRadioId(radio h){
  struct radio *this;
  this = (struct radio *) h;
  return this->id;
}


void apagaRadio(radio h){
  struct radio *this;
  this = (struct radio *) h;
  apagaFigura(this->circ);
  apagaTexto(this->text);
  free(this->id);
  free(h);
}
