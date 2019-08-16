#include "figura.h"
#include "funcoes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



struct figura{
  float x,y;
  char *cor1, *cor2, *espessura;
  int id;
  enum tipo_figura tipo;

  union{
    struct {
      float r;
    }circulo;

    struct {
      float w, h;
    }retangulo;
  }data;
};


figura criaCirculo(int id, float raio, float x, float y, char* cor1, char* cor2, char *espessura){
  struct figura *this;
  this = (struct figura *) malloc(sizeof(struct figura));
  this->id = id;
  this->x = x;
  this->y = y;
  this->tipo = CIRCULO;
  this->data.circulo.r = raio;
  this->cor1      = (char *) malloc (sizeof(char) * strlen(cor1) + 1);
  this->cor2      = (char *) malloc (sizeof(char) * strlen(cor2) + 1);
  this->espessura = (char *) malloc (sizeof(char) * strlen(espessura) + 1);
  strcpy(this->cor1, cor1);
  strcpy(this->cor2, cor2);
  strcpy(this->espessura, espessura);

  return (void *) this;
}


figura criaRetangulo(int id, float w, float h, float x, float y, char* cor1, char* cor2, char* espessura){
  struct figura *this;
  this = (struct figura *) malloc(sizeof(struct figura));
  this->id = id;
  this->x = x;
  this->y = y;
  this->tipo = RETANGULO;
  this->data.retangulo.w = w;
  this->data.retangulo.h = h;
  this->cor1 = (char *) malloc (sizeof(char) * strlen(cor1) + 1);
  this->cor2 = (char *) malloc (sizeof(char) * strlen(cor2) + 1);
  this->espessura = (char *) malloc (sizeof(char) * strlen(espessura) + 1);
  strcpy(this->cor1, cor1);
  strcpy(this->cor2, cor2);
  strcpy(this->espessura, espessura);

  return (void *) this;
}


figura criaBoundBox(figura fig, char *cor){
  struct figura *this;
  struct figura *aux;
  aux = (struct figura *) fig;
  this = (struct figura *) malloc(sizeof(struct figura));
  this->cor1 = (char *)malloc(sizeof(char) * 8);
  this->cor2 = (char *) malloc(sizeof(char) * (strlen(cor) + 1));
  this->espessura = (char *) malloc(sizeof(char) * 2);

  if(getTipoFigura(fig) == CIRCULO){
    this->id = -1;
    this->tipo = RETANGULO;
    this->x = aux->x - aux->data.circulo.r;
    this->y = aux->y - aux->data.circulo.r;
    this->data.retangulo.w = aux->data.circulo.r * 2;
    this->data.retangulo.h = aux->data.circulo.r * 2;
  }else{
    this->id = -1;
    this->tipo = CIRCULO;
    this->x = aux->x + (aux->data.retangulo.w/2);
    this->y = aux->y + (aux->data.retangulo.h/2);
    this->data.circulo.r = distancia(this->x, this->y, aux->x, aux->y);
  }
  strcpy(this->cor1, "magenta");
  strcpy(this->cor2, cor);
  strcpy(this->espessura, "2");

  return (void *) this;
}


enum tipo_figura getTipoFigura(figura fig){
  struct figura * this;
  this = (struct figura *)fig;
  return this->tipo;
}


int getId(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->id;
}



float getXfig(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->x;
}



float getYfig(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->y;
}


float getR(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->data.circulo.r;
}


float getW(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->data.retangulo.w;
}


float getH(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->data.retangulo.h;
}


char * getCor1(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->cor1;
}


char * getCor2(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->cor2;
}

char * getEspessura(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  return this->espessura;
}


void setCor1(figura fig, char *cor1){
  struct figura *this;
  this = (struct figura *) fig;
  free(this->cor1);
  this->cor1 = (char *) malloc(sizeof(char) * strlen(cor1) + 1);
  strcpy(this->cor1, cor1);
}

void setX(figura fig, float x){
  struct figura *this;
  this = (struct figura *) fig;
  this->x = x;
}

void setY(figura fig, float y){
  struct figura *this;
  this = (struct figura *) fig;
  this->y = y;
}

void apagaFigura(figura fig){
  struct figura *this;
  this = (struct figura *) fig;
  free(this->cor1);
  free(this->cor2);
  free(this->espessura);
  free(this);
}
