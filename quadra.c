#include "quadra.h"
#include "funcoes.h"
#include "svg.h"
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


int comparatorQuadra(quadra q1, quadra q2){
  struct quadra *this  = (struct quadra *) q1;
  struct quadra *this2 = (struct quadra *) q2;
  return comparatorFig(this->retan, this2->retan);
}

// void printSvgTreeQuad(quadra data, void* arquivo, float x, float y, int cor){

// }

char *quadraToString(quadra e){
  struct quadra *this;
  this = (struct quadra*) e;
  char *saida;
  saida = malloc(sizeof(char)*(floatlen(getXfig(this->retan)) + floatlen(getYfig(this->retan)) + strlen(this->cep) + 11));
  sprintf(saida, "CEP:%s\nX:%f\nY:%f", getCep(this), getXfig(this->retan), getYfig(this->retan));
  return saida;
}

void printaSvgQuadDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam){   
    static int lastS = 0;
    struct quadra *w = data;
    struct quadra *h = data2;
    char color[20];
    if(cor == 0){
        strcpy(color,"black");      
    }else if(cor == 1)
    {
        strcpy(color,"red");
    }
    fprintf(getSvgArq(arquivoSVG),"\t<circle r='%d' cx='%d' cy='%d' stroke='%s' fill='%s' stroke-width='1.0' fill-opacity='1'/>\n",10,x,y,color,color);
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>cep = %s</text>\n",x,y+15,getCep(w));
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>x = %f</text>\n",x,y+20,getXfig(w->retan));
    fprintf(getSvgArq(arquivoSVG),"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>y = %f</text>\n",x,y+25,getYfig(w->retan));

    if(getXfig(w->retan) < lastS)
        //printf("\nMUTE\n");
    lastS = getXfig(w->retan);
    //return NULL;
}

//TAD QUADRA
int verificaDentroQuadra(void* data,void** vetPontos,int tam,void* T,void *arquivoSVG,void* node){
    struct quadra *quadra = data;
    void* p1 = NewPoint(getXfig(quadra),getYfig(quadra));
    void* p2 = NewPoint(getXfig(getQuadraRect(quadra))+getW(getQuadraRect(quadra)),getYfig(getQuadraRect(quadra)));
    void* p3 = NewPoint(getXfig(quadra->retan),getYfig(quadra->retan)+getH(quadra->retan));
    void* p4 = NewPoint(getXfig(quadra->retan)+getW(quadra->retan),getYfig(quadra->retan)+getH(quadra->retan));
    if(InsidePolygon(vetPontos,tam,p1) == 1 && InsidePolygon(vetPontos,tam,p2) == 1 && InsidePolygon(vetPontos,tam,p3) == 1 && InsidePolygon(vetPontos,tam,p4) == 1){
        escreveLinha(getSvgArq(arquivoSVG),"\t<rect x='%f' y='%f' width='%f' height='%f' stroke='%s' fill='%s' stroke-width='5'/>\n",getXfig(quadra->retan),getYfig(quadra->retan),getW(quadra->retan),getH(quadra->retan),getCor1(quadra->retan),getCor2(quadra->retan));
    }
    return 0;
}
