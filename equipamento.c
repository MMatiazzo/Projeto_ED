#include "equipamento.h"
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

typedef struct{
  texto text;
  char *id;
  figura circ;
  enum tipo_equipamento tipo;
}equip_t;


equipamento criaHidrante(char *id, double x, double y, char *cor1, char *cor2, char *espessura){
  equip_t *this;
  this = (equip_t *) malloc(sizeof(equip_t));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-5, 8.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "H");
  this->tipo = HIDRANTE;
  strcpy(this->id, id);
  return (equipamento) this;
}


equipamento criaSemaforo(char *id, double x, double y, char *cor1, char *cor2, char *espessura){
  equip_t *this;
  this = (equip_t *) malloc(sizeof(equip_t));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-4, 8.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "S");
  this->tipo = SEMAFORO;
  strcpy(this->id, id);
  return (equipamento) this;
}


equipamento criaRadio(char *id, double x, double y, char *cor1, char *cor2, char *espessura){
  equip_t *this;
  this = (equip_t *) malloc(sizeof(equip_t));
  this->id = (char *)malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-3, 8.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "R");
  this->tipo = RADIO;
  strcpy(this->id, id);
  return (equipamento) this;
}


figura getEquipamentoCirculo(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  return this->circ;
}


texto getEquipamentoTexto(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  return this->text;
}


char* getEquipamentoId(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  return this->id;
}


enum tipo_equipamento getEquipamentoTipo(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  return this->tipo;
}


int equipamentoComparator(equipamento e1, equipamento e2, float x, float y){
  float x1,y1,x2,y2,d1,d2;
  equip_t *c1;
  equip_t *c2;
  c1 = (equip_t *)e1;
  c2 = (equip_t *)e2;
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



void apagaEquipamento(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  apagaFigura(this->circ);
  apagaTexto (this->text);
  free(this->id);
  free(this);
}
