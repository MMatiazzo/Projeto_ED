#include "segmento.h"
#include "PontoBrl.h"
#include "hash.h"
#include "Tree.h"
#include "funcoes.h"
#include "pessoa.h"
#include "executor.h"
#include "comando.h"
#include "svg.h"
#include "figura.h"
#include "texto.h"
#include "predio.h"
#include "heapsort.h"
#include "equipamento.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//Segmento contOeste,contLeste,contSul,contNorte,contOesteB;
//float xFoco, yFoco;


typedef struct{
  figura circ;
  arquivo txt;
  char *cor;
}cbq_t;

typedef struct{
  float x, y, w, h, dx, dy;
}trns_t;

typedef struct{
  char *string;
  float r;
  float x_eq;
  float y_eq;
  figura circ;
  arvore quadras;
  hash_table quadras_hash;
  arquivo txt;
}dq_t;

typedef struct{
  char *cep;
  arvore morador;
  arquivo txt;
  hash_table pessoa;
}m_t;


void trnsQuadra(void *auxiliar, quadra quad);
void trnsEquip(void *auxiliar, equipamento equip);
void imprimeMoradoresAux(void *aux1, void* aux2);
double min(double vet[], int size);
double max(double vet[], int size);


float MAX(float a, float b){
  if(a > b)
    return a;
  else
    return b;
}


float MIN(float a, float b){
  if(a < b)
    return a;
  else
    return b;
}

double min(double vet[], int size) {
    double minimo = vet[0];
    for(int i=1;i<size;i++){
        if(vet[i]<minimo)
            minimo=vet[i];
    }
    return minimo;
}

double max(double vet[], int size) {
    double maximo = vet[0];
    for(int i=1;i<size;i++){
        if(vet[i]>maximo)
            maximo=vet[i];
    }
    return maximo;
}

/*
double distanciaL1(double x1, double y1, double x2, double y2) {
	return fabs(x1-x2)+fabs(y1-y2);
}

double calcularAngulo(Ponto p, double x, double y) {
	double ang = atan2(Ponto_get_y(p) - y, Ponto_get_x(p) - x);
	//if(ang < 0) ang+=2*M_PI;
	return ang;
}*/

// int lado(Ponto A, Ponto B, Ponto C) {
//     int S;
//     S = getXPonto(A)*getYPonto(C) - getYPonto(A)*getXPonto(C) + getYPonto(A)*getXPonto(B)
//         - getXPonto(A)*getYPonto(B) + getXPonto(C)*getYPonto(B) - getYPonto(C)*getXPonto(B);

//     if(S<0) return -1;
//     else if(S>0) return 1;
//     else return 0;
// }

// int seInterceptam(Ponto A, Ponto B, Ponto C, Ponto D) {
//     int abc,abd,cda,cdb;

//     abc = lado(A,B,C);
//     abd = lado(A,B,D);
//     cda = lado(C,D,A);
//     cdb = lado(C,D,B);

//     return ((abc*abd<=0) && (cda*cdb<=0));
// }

int contaParametros(char * entrada){
  int total = 0;
  char *fre, *tok;
  tok = (char *) malloc(sizeof(char) * strlen(entrada) + 1);
  strcpy(tok, entrada);
  fre = tok;
  tok = strtok(tok, " ");

  while(strtok(NULL, " ") != NULL){
    total++;
  }

  free(fre);
  return total;
}


char **retornaParametros(char *entrada){
  char *fre, *token;
  char **parametros;
  int i = 0,total = contaParametros(entrada);
  parametros = (char **) malloc(sizeof(char *) * total);
  token = (char *) malloc(sizeof(char) * strlen(entrada) + 1);
  fre = token;
  strcpy(token, entrada);

  token = strtok(token, " ");

  while(token = strtok(NULL, " ")){
    parametros[i] = (char *) malloc(sizeof(char) * strlen(token) + 1);
    strcpy(parametros[i], token);
    i++;
  }

  free(fre);
  if(!i) return NULL;
  else return parametros;


}

int comparatorFoco(equipamento eq1, equipamento eq2, float x, float y){
  float x1,x2,y1,y2,d1,d2;
  x1 = getXfig(getEquipamentoCirculo(eq1));
  y1 = getYfig(getEquipamentoCirculo(eq1));
  x2 = getXfig(getEquipamentoCirculo(eq2));
  y2 = getYfig(getEquipamentoCirculo(eq2));
  d1 = distancia(x1,y1,x,y);
  d2 = distancia(x2,y2,x,y);
  if(d1 > d2){
    return 1;
  }else if(d2 > d1){
    return -1;
  }
  return 0;
}

char *colocaBarraFinal(char *linha){
  int tamanho = strlen(linha);
  char *aux;
  aux = (char *) malloc(sizeof(char) * strlen(linha) + 2);
  if(linha[tamanho - 1] != '/'){
    sprintf(aux, "%s/", linha);
  }else{
    sprintf(aux, "%s", linha);
  }
  return aux;
}

char *removeExtensao(char * nome){
  char *apenas_nome;
  char *aux;
  aux = (char *) malloc(sizeof(char) * strlen(nome) + 1);
  strcpy(aux, nome);
  aux = strtok(aux, ".");
  apenas_nome = (char *)malloc(sizeof(char) * strlen(aux) + 1);
  strcpy(apenas_nome, aux);
  free(aux);
  return apenas_nome;

}


int pesquisaNx(arquivo arq, int *nx){
  char *linha_atual;
  int total_linhas = 0;

  for(int i = 0; i < 7; i++){
    nx[i] = 1000;
  }

  while(linha_atual = lerLinha(arq)){
    if(linha_atual[0] == 'n'){
      sscanf(linha_atual, "nx %d %d %d %d %d %d %d", &nx[0], &nx[1], &nx[2], &nx[3], &nx[4], &nx[5], &nx[6]);
    }
    free(linha_atual);
    total_linhas++;
  }
  resetaArquivo(arq);
  return total_linhas;
}


int contaLinhas(arquivo arq){
  int total_linhas = 0;
  char *aux;

  while(aux = lerLinha(arq)){
    total_linhas++;
    free(aux);
  }
  resetaArquivo(arq);
  return total_linhas;
}


int floatlen(float x){
  char str[15];
  sprintf(str, "%f", x);
  return (strlen(str));
}


char *criaLinha(float x1, float y1, float x2, float y2){
  char *linha;
  linha = (char *) malloc(sizeof(char) * (floatlen(x1) + floatlen(y1) + floatlen(x2) + floatlen(y2) +73));
  sprintf(linha,"<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" style=\"stroke:red;stroke-width:4\"/>", x1, y1, x2, y2);
  return linha;
}


int sobrepoem(figura figura1, figura figura2){
  enum tipo_figura tipo1, tipo2;
  tipo1 = getTipoFigura(figura1);
  tipo2 = getTipoFigura(figura2);
  float x1, x2, y1, y2, w, h, raio;
  float soma_raio;

  if((tipo1 == CIRCULO) && (tipo2 == CIRCULO)){
    centroMassa(figura1, &x1, &y1);
    centroMassa(figura2, &x2, &y2);
    soma_raio = getR(figura1);
    soma_raio += getR(figura2);
    return (soma_raio > distancia(x1, y1, x2, y2));
  }

  if((tipo1 == RETANGULO) && (tipo2 == RETANGULO)){
    x1 = getXfig(figura2);
    y1 = getYfig(figura2);
    x2 = x1 + getW(figura2);
    y2 = y1 + getH(figura2);
    if(pontoInterno(figura1, x1, y1) || pontoInterno(figura1, x2, y1) || pontoInterno(figura1, x1, y2) || pontoInterno(figura1, x2, y2))
      return 1;
  }

  if((tipo1 != tipo2)){
    float deltaX, deltaY;

    if(tipo1 == RETANGULO){
      x1 = getXfig(figura1);
      y1 = getYfig(figura1);
      w = getW(figura1);
      h = getH(figura1);
      x2 = getXfig(figura2);
      y2 = getYfig(figura2);
      raio = getR(figura2);
    }else{
      x1 = getXfig(figura2);
      y1 = getYfig(figura2);
      w = getW(figura2);
      h = getH(figura2);
      x2 = getXfig(figura1);
      y2 = getYfig(figura1);
      raio = getR(figura1);
    }
    deltaX = x2 - MAX(x1, MIN(x2, x1 + w));
    deltaY = y2 - MAX(y1, MIN(y2, y1 + h));
    return (deltaX * deltaX + deltaY * deltaY) <= (raio * raio);
  }
}


void centroMassa(figura fig, float *x, float *y){
  int tipo = getTipoFigura(fig);


  switch (tipo) {
    case CIRCULO:
      *x = getXfig(fig);
      *y = getYfig(fig);
      break;

    case RETANGULO:
      *x = getXfig(fig) + (getW(fig)/2);
      *y = getYfig(fig) + (getH(fig)/2);
  }
}


float distancia(float x1, float y1, float x2, float y2){
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


int pontoInterno(figura fig, float x1, float y1){
  float x2, y2, r, w, h;
  x2 = getXfig(fig);
  y2 = getYfig(fig);


  switch(getTipoFigura(fig)){

    case CIRCULO:
      r = getR(fig);
      if(distancia(x1, y1, x2, y2) < r)
        return 1;
    break;

    case RETANGULO:

    w = getW(fig);
    h = getH(fig);
    if((x2 < x1) && (x1 < x2 + w) && (y2 < y1) && (y1 < y2 + h))
      return 1;
    break;  
  }

  return 0;
}


char* sobrepoeRetangulo(figura figura1, figura figura2){
  float maxX, maxY, minX, minY;
  float cx, cy, r;
  float rx, ry, w, h;
  float dasharray = 7;
  int sobr;
  char *aux;
  enum tipo_figura tipo1, tipo2;
  tipo1 = getTipoFigura(figura1);
  tipo2 = getTipoFigura(figura2);
  if(sobrepoem(figura1, figura2))
    dasharray = 0;

  if(tipo1 == RETANGULO && tipo2 == RETANGULO){
    float rx2, ry2, w2, h2;
    rx = getXfig(figura1);
    ry = getYfig(figura1);
    w = getW(figura1);
    h = getH(figura1);
    rx2 = getXfig(figura2);
    ry2 = getYfig(figura2);
    w2 = getW(figura2);
    h2 = getH(figura2);
    minX = MIN(rx, rx2);
    maxX = MAX(rx+w, rx2+w2);
    minY = MIN(ry, ry2);
    maxY = MAX(ry+h, ry2+h2);

  }

  if(tipo1 == CIRCULO && tipo2 == CIRCULO){
    float cx2, cy2, r2;
    cx = getXfig(figura1);
    cy = getYfig(figura1);
    r = getR(figura1);
    cx2 = getXfig(figura2);
    cy2 = getYfig(figura2);
    r2 = getR(figura2);
    minX = MIN(cx - r, cx2 - r2);
    maxX = MAX(cx + r, cx2 + r2);
    minY = MIN(cy - r, cy2 - r2);
    maxY = MAX(cy + r, cy2 + r2);
  }

  if(tipo1 != tipo2){
    if(tipo1 == RETANGULO){
      rx = getXfig(figura1);
      ry = getYfig(figura1);
      w = getW(figura1);
      h = getH(figura1);
      cx = getXfig(figura2);
      cy = getYfig(figura2);
      r = getR(figura2);
    }else{
      rx = getXfig(figura2);
      ry = getYfig(figura2);
      w = getW(figura2);
      h = getH(figura2);
      cx = getXfig(figura1);
      cy = getYfig(figura1);
      r = getR(figura1);
    }
    minX = MIN(cx - r, rx);
    maxX = MAX(cx + r, rx + w);
    minY = MIN(cy - r, ry);
    maxY = MAX(cy + r, ry + h);
  }

  aux = (char *) malloc(sizeof(char) * ( floatlen(minX) + floatlen(minY) + floatlen(maxX - minX) + floatlen(maxY - minY) + 99));
  sprintf(aux, "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.1f\" height=\"%.1f\" stroke=\"magenta\" stroke-width=\"5\" stroke-dasharray=\"10,%f\" fill=\"none\"/>",
          minX, minY, maxX-minX, maxY-minY, dasharray);
  return aux;

}


int RectDentroCirculoHelper(float r, float x, float y, float x1, float y1){
  float x2, y2, w, h;
  x2 = x;
  y2 = y;
  if(distancia(x1, y1, x2, y2) < r)
    return 1;
}


void RectDentroCirculo(void *auxiliar, quadra quadra_atual){
  figura circulo = ((cbq_t *)auxiliar)->circ;
  figura rect = getQuadraRect(quadra_atual);
  arquivo txt = ((cbq_t *)auxiliar)->txt;
  char *cor = ((cbq_t *)auxiliar)->cor;

  float xc = getXfig(circulo);
  float yc = getYfig(circulo);
  float r = getR(circulo);
  float x,y,w,h;
  float x1,y1,x2,y2,x3,y3,x4,y4;
  x = getXfig(rect);
  y = getYfig(rect);
  w = getW(rect);
  h = getH(rect);
  x1 = x    ; y1 = y;
  x2 = x + w; y2 = y;
  x3 = x    ; y3 = y + h;
  x4 = x + w; y4 = y + h;
  if(RectDentroCirculoHelper(r, xc, yc, x1, y1) && RectDentroCirculoHelper(r, xc, yc, x2, y2) &&
          RectDentroCirculoHelper(r, xc, yc, x3, y3) && RectDentroCirculoHelper(r, xc, yc, x4, y4)){
    setCor1(rect, cor);
    escreveLinha(txt, "Quadra cor alterada - cep: %s\n", getCep(quadra_atual) );
  }
  
}

void RectDentroCirculoDq(void *auxiliar, quadra quadra_atual){
  figura circulo = ((dq_t*)auxiliar)->circ;
  figura rect = getQuadraRect(quadra_atual);
  float xc = getXfig(circulo);
  float yc = getYfig(circulo);
  float r = getR(circulo);
  float x,y,w,h;
  float x1,y1,x2,y2,x3,y3,x4,y4;
  x = getXfig(rect);
  y = getYfig(rect);
  w = getW(rect);
  h = getH(rect);
  x1 = x    ; y1 = y;
  x2 = x + w; y2 = y;
  x3 = x    ; y3 = y + h;
  x4 = x + w; y4 = y + h;
  if(RectDentroCirculoHelper(r, xc, yc, x1, y1) && RectDentroCirculoHelper(r, xc, yc, x2, y2) &&
          RectDentroCirculoHelper(r, xc, yc, x3, y3) && RectDentroCirculoHelper(r, xc, yc, x4, y4)){
    escreveLinha(((dq_t*)auxiliar)->txt, "Quadra removida - cep: %s\n", getCep(quadra_atual) );
    deletaArvore(((dq_t*)auxiliar)->quadras,getCep(quadra_atual),getCep);
    remove_hash(((dq_t*)auxiliar)->quadras_hash,getCep(quadra_atual));
  }
  
}


void RectDentroCirculoL2(void *auxiliar, quadra quadra_atual){
figura rect = getQuadraRect(quadra_atual);
float r1,x1,y1;
r1 = ((dq_t*)auxiliar)->r;
x1 = ((dq_t*)auxiliar)->x_eq;
y1 = ((dq_t*)auxiliar)->y_eq;
float x2, y2, w2, h2;
x2 = getXfig(rect);
y2 = getYfig(rect);
w2 = getW(rect);
h2 = getH(rect);

  if(fabs(x2-x1)+fabs(y2-y1) <= r1)
    {
        if(fabs((x2+w2)-x1)+fabs(y2-y1)<=r1)
        {
            if(fabs(x2-x1)+fabs((y2+h2)-y1)<=r1)
            {
                if(fabs((x2+w2)-x1)+fabs((y2+h2)-y1)<=r1)
                {
                    escreveLinha(((dq_t*)auxiliar)->txt, "Quadra removida - cep: %s\n",getCep(quadra_atual));
                    deletaArvore(((dq_t*)auxiliar)->quadras,getCep(quadra_atual),getCep);
                    remove_hash(((dq_t*)auxiliar)->quadras_hash,getCep(quadra_atual));
                    return;
                }
                return;
            }
            return;
        }
        return;
    }
    return;
}


void cbqFunc(arvore a, float x, float y, float r, char *cstrk, arquivo txt){

  figura *circ = criaCirculo(-20,r,x,y,"red","red","1");
  cbq_t *auxiliar = malloc(sizeof(cbq_t));
  auxiliar->circ = circ;
  auxiliar->txt = txt;
  auxiliar->cor = cstrk;
  percorreArvore(a, RectDentroCirculo, auxiliar);
    
}


void dqFunc(svg arq_svg, arvore quadras,hash_table quadras_hash, hash_table hidrantes, hash_table semaforos, hash_table radios, char *tipoL, char *id, float r, arquivo txt){
  //node lista_node;
  float x_eq, y_eq;
  void *aux;
  char *text, *retorno = NULL;
  figura rect;
  item item_atual;
  quadra quadra_atual;


    if((item_atual = getHashTable(hidrantes, id))){
      item_atual = getEquipamentoCirculo(item_atual);
      x_eq = getXfig(item_atual);
      y_eq = getYfig(item_atual);
    }else
      if((item_atual = getHashTable(semaforos, id))){
        item_atual = getEquipamentoCirculo(item_atual);
        x_eq = getXfig(item_atual);
        y_eq = getYfig(item_atual);
    }else
      if((item_atual = getHashTable(radios, id))){
        item_atual = getEquipamentoCirculo(item_atual);
        x_eq = getXfig(item_atual);
        y_eq = getYfig(item_atual);
    }

    text = (char *)malloc(sizeof(char) * 100);
    sprintf(text, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"11\" stroke=\"red\" fill=\"#044B94\" stroke-width=\"2\"/>",x_eq, y_eq);
    escreveLinhaSVG(arq_svg, text);
    sprintf(text, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"13\" stroke=\"red\" fill=\"#044B94\" stroke-width=\"2\"/>",x_eq, y_eq);
    escreveLinhaSVG(arq_svg, text);



    if(!strcmp(tipoL, "L2")){

        dq_t *auxiliar = malloc(sizeof(dq_t));
        auxiliar->r = r;
        auxiliar->x_eq = x_eq;
        auxiliar->y_eq = y_eq;
        auxiliar->circ = criaCirculo(-2,r,x_eq,y_eq,"black","black","1.0");
        auxiliar->quadras_hash = quadras_hash;
        auxiliar->quadras = quadras;
        auxiliar->txt = txt;
        percorreArvore(quadras,RectDentroCirculoDq,auxiliar);
        
    }else{


      dq_t *auxiliar = malloc(sizeof(dq_t));
      auxiliar->r = r;
      auxiliar->x_eq = x_eq;
      auxiliar->y_eq = y_eq;
      auxiliar->circ = criaCirculo(-2,r,x_eq,y_eq,"black","black","1.0");
      auxiliar->quadras_hash = quadras_hash;
      auxiliar->quadras = quadras;
      auxiliar->txt = txt;
      percorreArvore(quadras,RectDentroCirculoL2,auxiliar);

    }
}

void trnsEquip(void *auxiliar, equipamento equip){
  float x,y,w,h,dx,dy;
  x = ((trns_t*)auxiliar)->x;
  y = ((trns_t*)auxiliar)->y;
  w = ((trns_t*)auxiliar)->w;
  h = ((trns_t*)auxiliar)->h;
  dx = ((trns_t*)auxiliar)->dx;
  dy = ((trns_t*)auxiliar)->dy;
  
  figura circ = getEquipamentoCirculo(equip);
  void* texto; 
    if(x < getXfig(circ)+getR(circ))
    {
        if(x+w> getXfig(circ)+getR(circ))
        {
          if(y < getYfig(circ)-getR(circ))
            {
              if(y+h>getYfig(circ)+getR(circ))
              {
                setX(circ, getXfig(circ)+dx);
                setY(circ,getYfig(circ)+dy);
                texto = getEquipamentoTexto(equip);
                setXtexto(texto,getXtexto(texto) + dx);
                setYtexto(texto,getYtexto(texto) + dy);
              }
            }
        }
    }
}

void trnsQuadra(void* auxiliar, quadra quad){
  figura rect;
  float x,y,w,h,dx,dy;
  x = ((trns_t*)auxiliar)->x;
  y = ((trns_t*)auxiliar)->y;
  w = ((trns_t*)auxiliar)->w;
  h = ((trns_t*)auxiliar)->h;
  dx = ((trns_t*)auxiliar)->dx;
  dy = ((trns_t*)auxiliar)->dy;

  rect = getQuadraRect(quad);

    if(x < getXfig(rect))
    {
        if(x+w> getXfig(rect)+getW(rect))
        {
          if(y < getYfig(rect))
            {
              if(y+h>getYfig(rect)+getH(rect))
              {
                setX(rect, getXfig(rect)+dx);
                setY(rect, getYfig(rect)+dy);
              }
            }
        }
    }
}

char *translocafunc(svg arq_svg, arvore quadras, arvore hidrantes, arvore semaforos, arvore radios, float x, float y, float w, float h, float dx, float dy){
  trns_t *auxiliar = malloc(sizeof(trns_t));
  auxiliar->x = x;
  auxiliar->y = y;
  auxiliar->w = w;
  auxiliar->h = h;
  auxiliar->dx = dx;
  auxiliar->dy = dy;

  percorreArvore(hidrantes,trnsEquip,auxiliar);
  percorreArvore(semaforos,trnsEquip,auxiliar);
  percorreArvore(radios,trnsEquip,auxiliar);
  percorreArvore(quadras,trnsQuadra,auxiliar);

}


int retornaDistanciaL1(float r1, float x1, float y1, float x2, float y2, float w2, float h2)
{
    if(fabs(x2-x1)+fabs(y2-y1) <= r1)
    {
        if(fabs((x2+w2)-x1)+fabs(y2-y1)<=r1)
        {
            if(fabs(x2-x1)+fabs((y2+h2)-y1)<=r1)
            {
                if(fabs((x2+w2)-x1)+fabs((y2+h2)-y1)<=r1)
                {
                    return 1;
                }
                return 0;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}


void** processaBB(arvore figuras, char* cor){
  void* fig_node;
  figura fig_atual;
  void **vetor_fig = arvoreToArray(figuras);
  void **vetor_bb = malloc(sizeof(void *) * getArvoreTamanho(figuras));
  for(int i = 0; i < getArvoreTamanho(figuras); i++){
    vetor_bb[i] = criaBoundBox(vetor_fig[i], cor);
  }
  free(vetor_fig);
  return  vetor_bb;
}

char *procuraObj(hash_table quadrs, hash_table hidrs, hash_table sems, hash_table rads, char *id){
  void *aux;
  if((aux = getHashTable(quadrs, id))){
    aux = (char *) aux;
    return dadosQuadraTxt(aux);
  }
  if((aux = getHashTable(hidrs, id))){

    return dadosHidranteTxt(aux);
  }
  if((aux = getHashTable(sems, id))){

    return dadosSemaforoTxt(aux);
  }
  if((aux = getHashTable(rads, id))){

    return dadosRadioTxt(aux);
  }

  return NULL;
}


char *deleteObj(arvore quadrs, arvore hidrs, arvore sems, arvore rads, hash_table quadras_hash,
hash_table hidrante_hash, hash_table semaforos_hash, hash_table radios_hash, char *id){
  void *aux;
  char *retorno = NULL;
  if((aux = getHashTable(quadras_hash,id))){
    retorno = dadosQuadraTxt(aux);
    deletaArvore(quadrs,getCep(aux),getCep);
    remove_hash(quadras_hash, id);
    //apagaQuadra(aux);
  }
  if((aux = getHashTable(hidrante_hash , id))){
    retorno = dadosHidranteTxt(aux);
    deletaArvore(hidrs,getEquipamentoId(aux),getEquipamentoId);
    remove_hash(hidrante_hash,id);
    //apagaEquipamento(aux);
  }
  if((aux = getHashTable(semaforos_hash ,id))){
    retorno = dadosSemaforoTxt(aux);
    deletaArvore(sems,getEquipamentoId(aux),getEquipamentoId);
    remove_hash(semaforos_hash,id);
    //apagaEquipamento(aux);
  }
  if((aux = getHashTable(radios_hash , id))){
    retorno = dadosRadioTxt(aux);
    deletaArvore(rads,getEquipamentoId(aux),getEquipamentoId);
    remove_hash(radios_hash,id);
    //apagaEquipamento(aux);
  }

  return retorno;
}


char *dadosQuadraTxt(quadra q){
  char *cep;
  float x,y,w,h;
  figura aux;
  aux = getQuadraRect(q);
  cep = getCep(q);
  x = getXfig(aux);
  y = getYfig(aux);
  w = getW(aux);
  h = getH(aux);
  aux = (char *) malloc(sizeof(char) * strlen(cep) + floatlen(x) + floatlen(y) + floatlen(w) + floatlen(h) + 44);
  sprintf(aux, "Quadra   :    cep:%s    x:%f    y:%f    w:%f    h:%f\n", cep, x, y, w, h);
  return aux;
}


char *dadosHidranteTxt(equipamento h){
  char *id;
  float x,y;
  figura aux;
  aux = getEquipamentoCirculo(h);
  id = getEquipamentoId(h);
  x = getXfig(aux);
  y = getYfig(aux);
  aux = (char *) malloc(sizeof(char) * strlen(id) + floatlen(x) + floatlen(y) + 31);
  sprintf(aux, "Hidrante :    id:%s    x:%f    y:%f\n", id, x, y);
  return aux;
}


char *dadosSemaforoTxt(equipamento s){
  char *id;
  float x,y;
  figura aux;
  aux = getEquipamentoCirculo(s);
  id = getEquipamentoId(s);
  x = getXfig(aux);
  y = getYfig(aux);
  aux = (char *) malloc(sizeof(char) * strlen(id) + floatlen(x) + floatlen(y) + 31);
  sprintf(aux, "Semaforo :    id:%s    x:%f    y:%f\n", id, x, y);
  return aux;
}


char *dadosRadioTxt(equipamento r){
  char *id;
  float x,y;
  figura aux;
  aux = getEquipamentoCirculo(r);
  id = getEquipamentoId(r);
  x = getXfig(aux);
  y = getYfig(aux);
  aux = (char *) malloc(sizeof(char) * strlen(id) + floatlen(x) + floatlen(y) + 31);
  sprintf(aux, "Radio    :    id:%s    x:%f    y:%f\n", id, x, y);
  return aux;
}


char * todoTextoFunc(int total_parametros, char ** parametros){
  char *aux;
  int i, j = 0;
  i = 2;
  while(i < total_parametros){
    j+= strlen(parametros[i]) + 1;
    i++;
  }

  aux = malloc(sizeof(char) * j);
  aux[0] = '\0';

  i = 2;
  while(i < total_parametros){
    strcat(aux, parametros[i]);
    if(i < total_parametros - 1)
      strcat(aux, " ");
    i++;
  }
  return aux;
}


void trocaString(char **str1, char *str2){
  free(*str1);
  *str1 = (char *) malloc(sizeof(char) * strlen(str2) + 1);
  strcpy(*str1, str2);
}


void concatenaString(char **str1, char *str2){
  if(!*str1){
    *str1 = malloc(sizeof(char) * strlen(str2) + 2);
    sprintf(*str1,"%s\n", str2);
  }else{
    char *aux = *str1;
    *str1 = malloc(sizeof(char) * strlen(aux) + strlen (str2) + 2);
    sprintf(*str1, "%s%s\n", aux, str2);
    free(aux);
  }
}

char *fIFunction(svg arq_svg, void **vet_s, void **vet_h, int s_n, int h_n, float x, float y, int ns, float r){
  char *id, *aux = NULL, *aux2 = NULL, *tmp=NULL, *linha = NULL;
  figura fig_atual, fig_aux;
  float d;


  aux = malloc(sizeof(char) * 12);
  strcpy(aux, "SEMAFOROS:\n");

  for(int i=0; i < ns && i < s_n; i++){
    fig_atual = getEquipamentoCirculo(vet_s[i]);
    tmp = getEquipamentoId(vet_s[i]);
    fig_aux = criaCirculo(-8, getR(fig_atual) + 3, getXfig(fig_atual), getYfig(fig_atual), "green", "transparent", "4");
    linha = criaLinha(getXfig(fig_atual), getYfig(fig_atual), x, y);
    escreveLinhaSVG(arq_svg, linha);
    desenhaFigura(arq_svg, fig_aux);
    free(linha);
    apagaFigura(fig_aux);
    if(tmp)
    concatenaString(&aux,tmp);
  }


  for(int i=0; i < h_n;i++){
    fig_atual = getEquipamentoCirculo(vet_h[i]);
    d = distancia(getXfig(fig_atual),getYfig(fig_atual), x, y);

    if(d < r){
      tmp = getEquipamentoId(vet_h[i]);
      fig_aux = criaCirculo(-8, getR(fig_atual) + 3, getXfig(fig_atual), getYfig(fig_atual), "red", "transparent", "4");
      linha = criaLinha(getXfig(fig_atual), getYfig(fig_atual), x, y);
      escreveLinhaSVG(arq_svg, linha);
      desenhaFigura(arq_svg, fig_aux);
      free(linha);
      apagaFigura(fig_aux);
      if(tmp)
      concatenaString(&aux2,tmp);
    }
  }


    if(aux2){
      tmp = malloc(sizeof(char) * strlen(aux2) + strlen("HIDRANTES:\n") + 1);
      sprintf(tmp, "%s%s", "HIDRANTES:\n", aux2);
      free(aux2);
      aux2 = tmp;
    }
    if(aux2){
      concatenaString(&aux, aux2);
      free(aux2);
    }
    if(aux){
      tmp = malloc(sizeof(char)* strlen(aux) + strlen("fi") + floatlen(x) + floatlen(y) + 5 + floatlen(r) + 6);
      sprintf(tmp,"fi %f %f %d %f\n%s", x, y, ns, r, aux);
      free(aux);
      aux = tmp;
    }else{
      aux = malloc(sizeof(char)*  strlen("fi") + floatlen(x) + floatlen(y) + 5 + floatlen(r) + 6);
      sprintf(aux,"fi %f %f %d %f\n", x, y, ns, r);
    }

  return aux;

}



char *fSFunction(svg arq_svg, void **vet_s, hash_table quadras, int s_n, int k, char *cep, char *face, float num){
  char *aux, *tmp = NULL, *linha = NULL;
  float x,y;
  figura fig_atual = NULL, fig_aux;

  if(!(fig_atual = getHashTable(quadras, cep))){
    aux = malloc(sizeof(char) * strlen("fs") + strlen(cep) + strlen(face)  + floatlen(num) + strlen("     cep não encontrado\n") + 10);
    strcpy(aux,"     cep não encontrado\n");
    return aux;
  }

  fig_atual = getQuadraRect(fig_atual);
  x = getXfig(fig_atual);
  y = getYfig(fig_atual);

  if(face[0] == 'S'){
    x += num;
  }else if(face[0] == 'N'){
    x += num;
    y += getH(fig_atual);
  }else if(face[0] == 'L'){
    y += getH(fig_atual);
  }else if(face[0] == 'O'){
    x += getW(fig_atual);
    y += num;
  }

  heapsort(vet_s, s_n, comparatorFoco, x, y);
  aux = malloc(sizeof(char) * 12);
  strcpy(aux, "SEMAFOROS:\n");

  for(int i=0; i < k && i < s_n; i++){
    fig_atual = getEquipamentoCirculo(vet_s[i]);
    tmp = getEquipamentoId(vet_s[i]);
    fig_aux = criaCirculo(-8, getR(fig_atual) + 3, getXfig(fig_atual), getYfig(fig_atual), "purple", "transparent", "4");
    linha = criaLinha(getXfig(fig_atual), getYfig(fig_atual), x, y);
    escreveLinhaSVG(arq_svg, linha);
    desenhaFigura(arq_svg, fig_aux);
    free(linha);
    apagaFigura(fig_aux);
    if(tmp)
    concatenaString(&aux,tmp);
  }

  if(aux){
    tmp = malloc(sizeof(char) * strlen("fs") + strlen(cep) + strlen(face)  + floatlen(num) + strlen(aux) + 30);
    sprintf(tmp,"fs %d %s %s %f\n%s", k, cep, face, num, aux);
    free(aux);
    aux = tmp;
  }
  return aux;

}



char *fHFunction(svg arq_svg, void **vet_h, hash_table quadras, int h_n, int k, char *cep, char *face, float num){
  char *aux, *tmp = NULL, *linha = NULL;
  float x,y;
  figura fig_atual = NULL, fig_aux;

  if(!(fig_atual = getHashTable(quadras, cep))){
    aux = malloc(sizeof(char) * strlen("fh") + strlen(cep) + strlen(face)  + floatlen(num) + strlen("     cep não encontrado\n") + 10);
    strcpy(aux,"     cep não encontrado\n");
    return aux;
  }

  fig_atual = getQuadraRect(fig_atual);
  x = getXfig(fig_atual);
  y = getYfig(fig_atual);

  if(face[0] == 'S'){
    x += num;
  }else if(face[0] == 'N'){
    x += num;
    y += getH(fig_atual);
  }else if(face[0] == 'L'){
    y += num;
  }else if(face[0] == 'O'){
    x += getW(fig_atual);
    y += num;
  }

  heapsort(vet_h, h_n, comparatorFoco, x, y);
  aux = malloc(sizeof(char) * 12);
  strcpy(aux, "HIDRANTES:\n");

  int i = 0;
  if(k > 0){
    i = h_n - k;
  }else{
    k = k*-1;
    h_n = k;
  }
    for(;i<h_n;i++){
      fig_atual = getEquipamentoCirculo(vet_h[i]);
      tmp = getEquipamentoId(vet_h[i]);
      fig_aux = criaCirculo(-8, getR(fig_atual) + 3, getXfig(fig_atual), getYfig(fig_atual), "yellow", "transparent", "4");
      linha = criaLinha(getXfig(fig_atual), getYfig(fig_atual), x, y);
      escreveLinhaSVG(arq_svg, linha);
      desenhaFigura(arq_svg, fig_aux);
      free(linha);
      apagaFigura(fig_aux);
      if(tmp)
      concatenaString(&aux,tmp);
  }

  if(aux){
    tmp = malloc(sizeof(char) * strlen("fs") + strlen(cep) + strlen(face)  + floatlen(num) + strlen(aux) + 30);
    sprintf(tmp,"fh %d %s %s %f\n%s", k, cep, face, num, aux);
    free(aux);
    aux = tmp;
  }
  return aux;

}

void imprimeMoradoresAux(void *aux1, void* aux2){
  m_t *this = (m_t*) aux1;
  void *aux;
  if(!strcmp(getMoradiaCep(aux2),this->cep)){
    aux = getHashTable(this->pessoa,getMoradiaCpf(aux2));
    escreveLinha(this->txt,"DADOS MORADOR -> cpf = %s | nome = %s | sobrenome = %s | sexo = %c | nascimento = %s\n",getPessoaCpf(aux),getPessoaNome(aux),getPessoaSobrenome(aux),getPessoaSexo(aux),getPessoaNascimento(aux));
  }
}

void imprimeMoradores(arvore a, arquivo txt, char *cep, hash_table pessoas){
  m_t *this = malloc(sizeof(m_t));
  this->cep = malloc(sizeof(char)*strlen(cep)+1);
  strcpy(this->cep,cep);
  this->morador = a;
  this->txt = txt;
  this->pessoa = pessoas;
  percorreArvore(a,imprimeMoradoresAux,this);
}


int navegaArvore(arvore a, node ramo, char*(*toString)(void *)){
  char entrada[2];
  char *aux;
  printf("%s\n%s\n", (aux = toString(getNodeData(ramo))), getNodeCor(ramo) == 0 ? "black":"red");
  printf("\n\ne -> Filho Esquerdo | d -> Filho direito | p -> Pai | x -> SAIR\n");
  free(aux);
  scanf(" %[^\n]", entrada);
  if(entrada[0] == 'x')
    return 0;
  
  if(entrada[0] == 'e'){
    if(isNulo(a, getNodeLeft(ramo))){
      printf("RAMO NILL\n");
      if(!navegaArvore(a, ramo, toString)){
        return 0;
      }
    }
    else
      if(!navegaArvore(a, getNodeLeft(ramo), toString)){
        return 0;
      }

  }else if(entrada[0] == 'd'){
    if(isNulo(a, getNodeRight(ramo))){
      printf("RAMO NILL\n");
      if(!navegaArvore(a, ramo, toString)){
        return 0;
      }
    }else{
      if(!navegaArvore(a, getNodeRight(ramo), toString)){
        return 0;
      }
    }
  
  }else if(entrada[0] == 'p'){
    if(isNulo(a, getNodeParent(ramo))){
      printf("RAMO NILL\n");
      if(!navegaArvore(a, ramo, toString)){
        return 0;
      }
    }else{
      if(!navegaArvore(a, getNodeParent(ramo), toString)){
        return 0;
      }
    }
  }
  navegaArvore(a, ramo, toString);
}



int Y_PRINT_ARVORE = 15;

static void printOrdenadoTree(arvore a, int x1, node x, svg arq_svg) {
  figura fig;

    if (!isNulo(a, x)) {
        x1+=20;
    	printOrdenadoTree(a, x1, getNodeLeft(x), arq_svg);
      
      fprintf(arq_svg, "<circle r='5' cx='%d' cy='%d' stroke='black' fill='%s' stroke-width='1'/>\n", Y_PRINT_ARVORE, x1, getNodeCor(x) == 0 ? "black" : "red");
      char dados[150];
      fprintf(arq_svg, "<text x='%d' y='%d' fill='black' font-size='5'></text>\n", Y_PRINT_ARVORE, x1);

        Y_PRINT_ARVORE+=15;
    	printOrdenadoTree(a, x1, getNodeRight(x), arq_svg);
    }
}

void printTree(arvore a, svg arq_svg) {
    printOrdenadoTree(a, 0, getNodeLeft(getArvoreRaiz(a)), arq_svg);
}


//
