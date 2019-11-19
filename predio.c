#include "PontoBrl.h"
#include "svg.h"
#include "predio.h"
#include "funcoes.h"
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

float getPredioFrente(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->frente;
}

float getPredioMargem(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->margem;
}

float getPredioNum(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->num;
}

float getPredioProfundidade(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->profundidade;
}

char * getPredioFace(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->face;
}

char *getPredioCep(predio p){
  predio_t *this;
  this = (predio_t *)p;
  return this->cep;
}


void apagaPredio(predio p){
  predio_t *this = (predio_t *) p;
  free(this->cep);
  free(this->face);
  apagaFigura(this->rect);
  free(this);
}


int comparatorPredio(predio p1, predio p2){
  predio_t *this, *this2;
  this = (predio_t *) p1;
  this2 = (predio_t *) p2;
  return comparatorFig(this->rect, this2->rect);
}

void printaSvgPredioDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam){   
    static int lastS = 0;
    predio_t *w = data;
    predio_t *h = data2;
    char color[20];
    if(cor == 0){
        strcpy(color,"black");      
    }else if(cor == 1)
    {
        strcpy(color,"red");
    }
    fprintf(getSvgArq(arquivoSVG),"\t<circle r='%d' cx='%d' cy='%d' stroke='%s' fill='%s' stroke-width='1.0' fill-opacity='1'/>\n",10,x,y,color,color);
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>cep = %s</text>\n",x,y+15,w->cep);
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>x = %f</text>\n",x,y+20,getXfig(w->rect));
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>y = %f</text>\n",x,y+25,getYfig(w->rect));

    if(getXfig(w->rect) < lastS)
    lastS = getXfig(w->rect);
}

//TAD PREDIO
int verificaDentroPredio(void* data,void** vetPontos,int tam,void* T,void *arquivoSVG,void* node){
    struct predio *predio = data;
    void* p1 = NewPoint(getXfig(predio->rect),getYfig(predio->rect));
    void* p2 = NewPoint(getXfig(predio->rect)+getW(predio->rect),getYfig(predio->rect));
    void* p3 = NewPoint(getXfig(predio->rect),getYfig(predio->rect)+getH(predio->rect));
    void* p4 = NewPoint(getXfig(predio->rect)+getW(predio->rect),getYfig(predio->rect)+getH(predio->rect));
    if(InsidePolygon(vetPontos,tam,p1) == 1 && InsidePolygon(vetPontos,tam,p2) == 1 && InsidePolygon(vetPontos,tam,p3) == 1 && InsidePolygon(vetPontos,tam,p4) == 1){
        escreveLinha(getSvgArq(arquivoSVG),"\t<rect x='%f' y='%f' width='%f' height='%f' stroke='red' fill='red' stroke-width='5'/>\n",getXfig(predio->rect),getYfig(predio->rect),getW(predio->rect),getH(predio->rect));
    }
    return 0;
}