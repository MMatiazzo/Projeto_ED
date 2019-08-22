#include "predio.h"
#include <string.h>
#include <stdlib.h>

typedef struct predio{
  char *face, *cep;
  float frente, profundidade, margem, num;
  figura rect;
}predio_t;

predio criaPredio(char *cep, char *face, float num, float f, float p, float mrg, figura rectQuadra){
  predio_t *this = malloc(sizeof(predio_t));
  this->cep = malloc(sizeof(char)* strlen(cep) + 1);
  this->face = malloc(sizeof(char) * strlen(face) + 1);
  strcpy(this->cep, cep);
  strcpy(this->face, face);
  this->num = num;
  this->frente = f;
  this->profundidade = p;
  this->margem = mrg;

  float xQuadra, yQuadra, wQuadra, hQuadra, xPredio, yPredio, wPredio, hPredio;

  xQuadra = getXfig(rectQuadra);
  yQuadra = getYfig(rectQuadra);
  wQuadra = getW(rectQuadra);
  hQuadra = getH(rectQuadra);

  if(!strcmp(face, "S")){
    xPredio = xQuadra + num - (f/2);
    yPredio = yQuadra + mrg;
    wPredio = f;
    hPredio = p;
  }else if(!strcmp(face, "N")){
    xPredio = xQuadra + num - (f/2);
    yPredio = yQuadra + hQuadra - mrg - p;
    wPredio = f;
    hPredio = p;
  }else if(!strcmp(face, "L")){
    xPredio = xQuadra + wQuadra - mrg - p;
    yPredio = yQuadra + num -(f/2);
    wPredio = p;
    hPredio = f;
  }else if(!strcmp(face, "O")){
    xPredio = xQuadra + mrg;
    yPredio = yQuadra + num - (f/2);
    wPredio = p;
    hPredio = f;
  }

  this->rect = criaRetangulo(-7, wPredio, hPredio, xPredio, yPredio, "black", "blue", "1");


  return (predio) this;
}

figura getPredioRect(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->rect;
}


void apagaPredio(predio p){
  predio_t *this = (predio_t *) p;
  free(this->cep);
  free(this->face);
  apagaFigura(this->rect);
  free(this);
}
