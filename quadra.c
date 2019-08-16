#include "quadra.h"
#include <string.h>
#include <stdlib.h>

struct quadra{
  char *cep;
  figura retan;
};

quadra criaQuadra(char * cep, float x, float y, float w, float h, char *cor1, char *cor2, char *espessura){
  struct quadra *this;
  this = (struct quadra *) malloc(sizeof(struct quadra));
  this->cep = (char *) malloc(sizeof(char) * strlen(cep) + 1);
  strcpy(this->cep, cep);
  this->retan = criaRetangulo(-2, w, h, x, y, cor1, cor2, espessura);
  return (quadra) this;
}

char * getCep(quadra q){
  struct quadra * this;
  this = (struct quadra *) q;
  return this->cep;
}

figura getQuadraRect(quadra q){
  struct quadra * this;
  this = (struct quadra *) q;
  return this->retan;
}



void apagaQuadra(quadra q){
  struct quadra *this;
  this = (struct quadra*) q;
  free(this->cep);
  apagaFigura(this->retan);
  free(this);
}
