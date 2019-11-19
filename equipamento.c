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
  this->circ = criaCirculo(-5, 5.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "H");
  this->tipo = HIDRANTE;
  strcpy(this->id, id);
  return (equipamento) this;
}


equipamento criaSemaforo(char *id, double x, double y, char *cor1, char *cor2, char *espessura){
  equip_t *this;
  this = (equip_t *) malloc(sizeof(equip_t));
  this->id = (char *) malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-4, 5.0, x, y, cor1, cor2, espessura);
  this->text = criaTexto(x-5, y+5, "S");
  this->tipo = SEMAFORO;
  strcpy(this->id, id);
  return (equipamento) this;
}


equipamento criaRadio(char *id, double x, double y, char *cor1, char *cor2, char *espessura){
  equip_t *this;
  this = (equip_t *) malloc(sizeof(equip_t));
  this->id = (char *)malloc(sizeof(char) * strlen(id) + 1);
  this->circ = criaCirculo(-3, 5.0, x, y, cor1, cor2, espessura);
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



int equipamentoComparator(equipamento e1, equipamento e2){
  equip_t *this, *this2;
  this = (equip_t *) e1;
  this2 = (equip_t *) e2;
  return comparatorFig(this->circ, this2->circ);
}
  
char *equipamentoToString(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  char *saida;
  saida = malloc(sizeof(char)*(floatlen(getXfig(this->circ)) + floatlen(getYfig(this->circ)) + strlen(this->id) + 10));
  sprintf(saida, "ID:%s\nX:%f\nY:%f", getEquipamentoId(this), getXfig(this->circ), getYfig(this->circ));
  return saida;
}


void apagaEquipamento(equipamento e){
  equip_t* this;
  this = (equip_t *) e;
  apagaFigura(this->circ);
  apagaTexto (this->text);
  free(this->id);
  free(this);
}
//cota de cauchy OK!, chebychevi, newto OK!, bissecção OK!, eliminação de gaus
void printaSvgHidDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam){   
    static int lastS = 0;
    equip_t *w = data;
    equip_t *h = data2;
    char color[20];
    if(cor == 0){
        strcpy(color,"black");      
    }else if(cor == 1)
    {
        strcpy(color,"red");
    }
    fprintf(arquivoSVG,"\t<circle r='%d' cx='%d' cy='%d' stroke='%s' fill='%s' stroke-width='1.0' fill-opacity='1'/>\n",10,x,y,color,color);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>id = %s</text>\n",x,y+15,w->id);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>x = %f</text>\n",x,y+20,getXfig(w->circ));
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>y = %f</text>\n",x,y+25,getYfig(w->circ));

    if(getXfig(w->circ) < lastS)
    lastS = getXfig(w->circ);
}
