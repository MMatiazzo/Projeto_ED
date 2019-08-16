#include "hidrante.h"
#include <stdlib.h>
#include <string.h>

struct hidra{
  texto text;
  figura circ;
  char *id;
};

hidrante criaHidrante(char *id, float x, float y, char *cor1, char *cor2, char *espessura){
  struct hidra *this;
  this = (struct hidra *) malloc(sizeof(struct hidra));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-5, 10.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "H");
  strcpy(this->id, id);
  return (hidrante) this;
}


figura getHidranteCirc(hidrante h){
  struct hidra *this;
  this = (struct hidra *) h;
  return this->circ;
}


texto getHidranteText(hidrante h){
  struct hidra *this;
  this = (struct hidra *) h;
  return this->text;
}

char *getHidranteId(hidrante h){
  struct hidra *this;
  this = (struct hidra *) h;
  return this->id;
}


void apagaHidrante(hidrante h){
  struct hidra *this;
  this = (struct hidra *) h;
  apagaFigura(this->circ);
  apagaTexto(this->text);
  free(this->id);
  free(h);
}
