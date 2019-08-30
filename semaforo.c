#include "semaforo.h"
#include "funcoes.h"
#include <stdlib.h>
#include <string.h>

typedef struct semaf{
  texto text;
  figura circ;
  char *id;
}sema_t;


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


int semaforoComparator(semaforo s1, semaforo s2, float x, float y){
  float x1,y1,x2,y2,d1,d2;
  sema_t *c1;
  sema_t *c2;
  c1 = (sema_t *)s1;
  c2 = (sema_t *)s2;
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

// int comparaSemaforoPonto(semaforo s1, semaforo s2, float x, float y){
//   struct semaf *c1;
//   struct semaf *c2;
//   float x1, y1, x2, y2, d1, d2;
//   c1 = (struct semaf *) s1;
//   c2 = (struct semaf *) s2;
//   printf("TESTE:%s\n", c1->id);
//   x1 = getXfig(c1->circ);
//   y1 = getYfig(c1->circ);
//   x2 = getXfig(c2->circ);
//   y2 = getYfig(c2->circ);
//   d1 = distancia(x1, y1, x, y);
//   d2 = distancia(x2, y2, x, y);
//   if(d1 > d2) return 1;
//   if(d1 < d2) return -1;
//   else return 0;
// }
