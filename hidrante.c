#include "hidrante.h"
#include "funcoes.h"
#include <stdlib.h>
#include <string.h>

typedef struct hidra{
  texto text;
  figura circ;
  char *id;
}hidra_t;

hidrante criaHidrante(char *id, float x, float y, char *cor1, char *cor2, char *espessura){
  struct hidra *this;
  this = (struct hidra *) malloc(sizeof(struct hidra));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-5, 8.0, x, y, cor1, cor2, espessura);
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


int hidranteComparator(hidrante s1, hidrante s2, float x, float y){
  float x1,y1,x2,y2,d1,d2;
  hidra_t *c1;
  hidra_t *c2;
  c1 = (hidra_t *)s1;
  c2 = (hidra_t *)s2;
  x1 = getXfig(c1->circ);
  y1 = getYfig(c1->circ);
  x2 = getXfig(c2->circ);
  y2 = getYfig(c2->circ);
  d1 = distancia(x1, y1, x, y);
  d2 = distancia(x2, y2, x, y);
  if(d1 > d2) return 1;
  if(d1 < d2) return -1;
  else return 0;
}
