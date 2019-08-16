#include "semaforo.h"
#include <stdlib.h>
#include <string.h>

struct semaf{
  texto text;
  figura circ;
  char *id;
};


semaforo criaSemaforo(char *id, float x, float y, char *cor1, char *cor2, char *espessura){
  struct semaf *this;
  this = (struct semaf *) malloc(sizeof(struct semaf));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-4, 8.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "S");
  strcpy(this->id, id);
  return (semaforo) this;
}



figura getSemaforoCirc(semaforo h){
  struct semaf *this;
  this = (struct semaf *) h;
  return this->circ;
}


texto getSemaforoText(semaforo h){
  struct semaf *this;
  this = (struct semaf *) h;
  return this->text;
}

char *getSemaforoId(semaforo h){
  struct semaf *this;
  this = (struct semaf *) h;
  return this->id;
}


void apagaSemaforo(semaforo h){
  struct semaf *this;
  this = (struct semaf *) h;
  apagaFigura(this->circ);
  apagaTexto(this->text);
  free(this->id);
  free(h);
}
