#include "funcoes.h"
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
  // printf("x1:%f - y1:%f - x2:%f - y2:%f\n", x1, y1, x2, y2);
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


    case RETANGULO:
    w = getW(fig);
    h = getH(fig);
    if((x2 < x1) && (x1 < x2 + w) && (y2 < y1) && (y1 < y2 + h))
      return 1;
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


figura procuraFigura(lista figuras, int id){
  node fig_node;
  figura fig_atual;
  fig_node = getInicioLista(figuras);

  while(temProximo(fig_node)){
    fig_atual = (figura) getItemProx(figuras, &fig_node);
    if(getId(fig_atual) == id)
      return fig_atual;
  }
  return NULL;
}


int RectDentroCirculoHelper(float r, float x, float y, float x1, float y1){
  float x2, y2, w, h;
  x2 = x;
  y2 = y;
  if(distancia(x1, y1, x2, y2) < r)
    return 1;
}


int RectDentroCirculo(float r, float xc, float yc, figura rect){
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
  return (RectDentroCirculoHelper(r, xc, yc, x1, y1) && RectDentroCirculoHelper(r, xc, yc, x2, y2) &&
          RectDentroCirculoHelper(r, xc, yc, x3, y3) && RectDentroCirculoHelper(r, xc, yc, x4, y4));
}


int RectDentroCirculoL2(float r1, float x1, float y1, figura rect){
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


char *cbqFunc(lista list, float x, float y, float r, char *cstrk){
  node lista_node;
  char *retorno = NULL, *aux;
  figura rect = NULL;
  quadra quadra_atual;
  lista_node = getInicioLista(list);

    while(temProximo(lista_node)){

      quadra_atual  = getItemProx(list, &lista_node);
      rect = getQuadraRect(quadra_atual);

      if(RectDentroCirculo(r, x, y, rect)){
        if(!retorno){
          retorno = (char *) malloc(sizeof(char) * 30 + strlen(getCep(quadra_atual)));
          sprintf(retorno, "Quadra cor alterada - cep: %s\n", getCep(quadra_atual));
        }else{
          aux = retorno;
          retorno = (char *) malloc(sizeof(char) * strlen(aux) + strlen(getCep(quadra_atual)) +30);
          sprintf(retorno, "%sQuadra cor alterada - cep: %s\n", aux, getCep(quadra_atual));
          free(aux);
        }
        setCor1(rect, cstrk);
      }
    }

    return retorno;
}


char *dqFunc(svg arq_svg, lista quadras, lista hidrantes, lista semaforos, lista radios, char *tipoL, char *id, float r){
  node lista_node;
  float x_eq, y_eq;
  void *aux;
  char *text, *retorno = NULL;
  figura rect;
  item item_atual;
  quadra quadra_atual;


    if((item_atual = percorreLista(hidrantes, PROCURA_HIDRANTE_ID, id))){
      item_atual = getEquipamentoCirculo(item_atual);
      x_eq = getXfig(item_atual);
      y_eq = getYfig(item_atual);
    }else
      if((item_atual = percorreLista(semaforos, PROCURA_SEMAFORO_ID, id))){
        item_atual = getEquipamentoCirculo(item_atual);
        x_eq = getXfig(item_atual);
        y_eq = getYfig(item_atual);
    }else
      if((item_atual = percorreLista(radios, PROCURA_RADIO_ID, id))){
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

      lista_node = getInicioLista(quadras);



        while(temProximo(lista_node)){
          aux = lista_node;
          quadra_atual  = getItemProx(quadras, &lista_node);
          rect = getQuadraRect(quadra_atual);

          if(RectDentroCirculo(r, x_eq, y_eq, rect)){
              aux = remove_lista(quadras, getPosic(aux));
              retorno = (char *) malloc(sizeof(char) * 30 + strlen(getCep(quadra_atual)));
              sprintf(retorno, "Quadra removida - cep: %s\n", getCep(quadra_atual));

          }
        }
    }else{

      lista_node = getInicioLista(quadras);



        while(temProximo(lista_node)){
          aux = lista_node;
          quadra_atual  = getItemProx(quadras, &lista_node);
          rect = getQuadraRect(quadra_atual);

          if(RectDentroCirculoL2(r, x_eq, y_eq, rect)){
            if(!retorno){
              apagaQuadra(remove_lista(quadras, getPosic(aux)));
              retorno = (char *) malloc(sizeof(char) * 30 + strlen(getCep(quadra_atual)));
              sprintf(retorno, "Quadra removida - cep: %s\n", getCep(quadra_atual));
            }

          }
        }
    }
}


char *translocafunc(svg arq_svg, lista quadras, lista hidrantes, lista semaforos, lista radios, float x, float y, float w, float h, float dx, float dy){
  node lista_node;
  void * semaforo_atual, *quadra_atual, *radio_atual, *hidrante_atual, *texto;
  figura circ, rect;

  lista_node = getInicioLista(semaforos);
  while(temProximo(lista_node))
  {
    semaforo_atual = getItemProx(semaforos,&lista_node);
    circ = getEquipamentoCirculo(semaforo_atual);
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
                texto = getEquipamentoTexto(semaforo_atual);
                setXtexto(texto,getXtexto(texto) + dx);
                setYtexto(texto,getYtexto(texto) + dy);
              }
            }
        }
    }
  }



  lista_node = getInicioLista(hidrantes);
  while(temProximo(lista_node))
  {
    hidrante_atual = getItemProx(hidrantes ,&lista_node);
    circ = getEquipamentoCirculo(hidrante_atual);
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
                texto = getEquipamentoTexto(hidrante_atual);
                setXtexto(texto,getXtexto(texto) + dx);
                setYtexto(texto,getYtexto(texto) + dy);
              }
            }
        }
    }
  }



  lista_node = getInicioLista(radios);
  while(temProximo(lista_node))
  {
    radio_atual = getItemProx(radios ,&lista_node);
    circ = getEquipamentoCirculo(radio_atual);
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
                texto = getEquipamentoCirculo(radio_atual);
                setXtexto(texto,getXtexto(texto) + dx);
                setYtexto(texto,getYtexto(texto) + dy);
              }
            }
        }
    }
  }


  lista_node = getInicioLista(quadras);
  while(temProximo(lista_node))
  {
    quadra_atual = getItemProx(quadras ,&lista_node);
    rect = getQuadraRect(quadra_atual);

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


lista processaBB(lista figuras, char* cor){
  node fig_node;
  lista figuras_bb;
  figura fig_atual;
  figuras_bb = criaLista(getTamanho(figuras));
  fig_node = getInicioLista(figuras);

  while(temProximo(fig_node)){
    fig_atual = (figura) getItemProx(figuras, &fig_node);
    figuras_bb = insere_lista(figuras_bb, criaBoundBox(fig_atual, cor));
  }
  return figuras_bb;
}


void *percorreLista(lista list, enum tipo_operacao tipo, void * auxiliar){
  node lista_node;
  int posicao_atual;
  void * item_atual;
  comando comando_atual;

  lista_node = getInicioLista(list);

    while(temProximo(lista_node)){
      posicao_atual = getPosic(lista_node);
      item_atual = getItemProx(list, &lista_node);
      switch (tipo) {

        case EXECUTA_COMANDO:
          comando_atual = criaComando(item_atual);
          if(comando_atual)
            executarComando(auxiliar, comando_atual);
          apagaComandoGeo(comando_atual);
          break;

        case EXECUTA_COMANDO_QRY:
          comando_atual = criaComando(item_atual);
          if(comando_atual)
            executarComandoQry(auxiliar, comando_atual);
          apagaComandoGeo(comando_atual);
          break;

        case DESENHA_FIGURA:
          desenhaFigura(auxiliar, item_atual);
          break;

        case DESENHA_QUADRA:
          desenhaQuadra(auxiliar, item_atual);
          break;

        case DESENHA_HIDRANTE:
          desenhaHidrante(auxiliar, item_atual);
          break;

        case DESENHA_SEMAFORO:
          desenhaSemaforo(auxiliar, item_atual);
          break;

        case DESENHA_RADIO:
          desenhaRadio(auxiliar, item_atual);
          break;

        case ESCREVE_TEXTO:
          escreveTexto(auxiliar, item_atual);
          break;

        case ESCREVE_LINHA_SVG:
          escreveLinhaSVG(auxiliar, item_atual);
          break;

        case ESCREVE_LINHA:
          escreveLinha(auxiliar, item_atual);
          break;

        case DELETA_QUADRA_ID:
          if(!strcmp(getCep(item_atual), auxiliar)){
            remove_lista(list, posicao_atual);
            return item_atual;
          }
          break;

        case DELETA_HIDRANTE_ID:
          if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
            remove_lista(list, posicao_atual);
            return item_atual;
          }

        case DELETA_SEMAFORO_ID:
          if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
            remove_lista(list, posicao_atual);
            return item_atual;
          }
          break;

        case DELETA_RADIO_ID:
          if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
            remove_lista(list, posicao_atual);
            return item_atual;
          }
          break;

          case PROCURA_QUADRA_ID:
            if(!strcmp(getCep(item_atual), auxiliar)){
              return item_atual;
            }
            break;

          case PROCURA_HIDRANTE_ID:
            if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
              return item_atual;
            }

          case PROCURA_SEMAFORO_ID:
            if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
              return item_atual;
            }
            break;

          case PROCURA_RADIO_ID:
            if(!strcmp(getEquipamentoId(item_atual), auxiliar)){
              return item_atual;
            }
            break;

          case DESENHA_PREDIO:
            desenhaPredio(auxiliar, item_atual);
          break;

          case DESENHA_MURO:
            desenhaLineSVG(auxiliar, item_atual);
            break;
      }
    }
    return NULL;
}


char *procuraObj(lista quadrs, lista hidrs, lista sems, lista rads, char *id){
  void *aux;
  if((aux = percorreLista(quadrs, PROCURA_QUADRA_ID  , id))){
    aux = (char *) aux;
    return dadosQuadraTxt(aux);
  }
  if((aux = percorreLista(hidrs , PROCURA_HIDRANTE_ID, id))){

    return dadosHidranteTxt(aux);
  }
  if((aux = percorreLista(sems  , PROCURA_SEMAFORO_ID, id))){

    return dadosSemaforoTxt(aux);
  }
  if((aux = percorreLista(rads  , PROCURA_RADIO_ID   , id))){

    return dadosRadioTxt(aux);
  }

  return NULL;
}


char *deleteObj(lista quadrs, lista hidrs, lista sems, lista rads, char *id){
  void *aux;
  char *retorno = NULL;
  if((aux = percorreLista(quadrs, DELETA_QUADRA_ID  , id))){
    retorno = dadosQuadraTxt(aux);
    apagaQuadra(aux);
  }
  if((aux = percorreLista(hidrs , DELETA_HIDRANTE_ID, id))){
    retorno = dadosHidranteTxt(aux);
    apagaEquipamento(aux);
  }
  if((aux = percorreLista(sems  , DELETA_SEMAFORO_ID, id))){
    retorno = dadosSemaforoTxt(aux);
    apagaEquipamento(aux);
  }
  if((aux = percorreLista(rads  , DELETA_RADIO_ID   , id))){
    retorno = dadosRadioTxt(aux);
    apagaEquipamento(aux);
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
    // printf("AUX:%s\n", aux);
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
    // printf("X:%f\nY:%f\n", getXfig(vet_s[i]), getYfig(vet_s[i]));
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
      // printf("AEEE:%s\n", aux2);
      // free?
      // }else break;
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
    // printf("%s\n", aux);
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



char *fSFunction(svg arq_svg, void **vet_s, lista quadras, int s_n, int k, char *cep, char *face, float num){
  char *aux, *tmp = NULL, *linha = NULL;
  float x,y;
  figura fig_atual = NULL, fig_aux;

  if(!(fig_atual = percorreLista(quadras, PROCURA_QUADRA_ID, cep))){
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

  heapsort(vet_s, s_n, equipamentoComparator, x, y);
  aux = malloc(sizeof(char) * 12);
  strcpy(aux, "SEMAFOROS:\n");

  for(int i=0; i < k && i < s_n; i++){
    fig_atual = getEquipamentoCirculo(vet_s[i]);
    tmp = getEquipamentoId(vet_s[i]);
    // printf("X:%f\nY:%f\n", getXfig(vet_s[i]), getYfig(vet_s[i]));
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



char *fHFunction(svg arq_svg, void **vet_h, lista quadras, int h_n, int k, char *cep, char *face, float num){
  char *aux, *tmp = NULL, *linha = NULL;
  float x,y;
  figura fig_atual = NULL, fig_aux;

  if(!(fig_atual = percorreLista(quadras, PROCURA_QUADRA_ID, cep))){
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
    y += getH(fig_atual);
  }else if(face[0] == 'O'){
    x += getW(fig_atual);
    y += num;
  }

  heapsort(vet_h, h_n, equipamentoComparator, x, y);
  aux = malloc(sizeof(char) * 12);
  strcpy(aux, "HIDRANTES:\n");

  int i = 0;
  if(k < 0){
    k = abs(k);
  }else{
    i = h_n - k;
    k = h_n;
  }
  printf("TESTE:%d | %d\n", i, k);
  for(; i < k && i < h_n; i++){
    fig_atual = getEquipamentoCirculo(vet_h[i]);
    tmp = getEquipamentoId(vet_h[i]);
    // printf("X:%f\nY:%f\n", getXfig(vet_s[i]), getYfig(vet_s[i]));
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









//
