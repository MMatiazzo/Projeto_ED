#ifndef _FUNCOES_H_
#define _FUNCOES_H_

#include "figura.h"
#include "equipamento.h"
#include "quadra.h"
#include "listaed.h"
#include "arquivo.h"
#include "svg.h"

enum tipo_operacao{
  EXECUTA_COMANDO,
  EXECUTA_COMANDO_QRY,
  DESENHA_FIGURA,
  DESENHA_QUADRA,
  DESENHA_HIDRANTE,
  DESENHA_SEMAFORO,
  DESENHA_PREDIO,
  DESENHA_MURO,
  DESENHA_RADIO,
  ESCREVE_TEXTO,
  ESCREVE_LINHA_SVG,
  ESCREVE_LINHA,
  DELETA_QUADRA_ID,
  DELETA_HIDRANTE_ID,
  DELETA_SEMAFORO_ID,
  DELETA_RADIO_ID,
  PROCURA_LISTA,
  PROCURA_QUADRA_ID,
  PROCURA_HIDRANTE_ID,
  PROCURA_SEMAFORO_ID,
  PROCURA_RADIO_ID,
};


float MIN(float a, float b);

float MAX(float a, float b);

int contaParametros(char *entrada);

char **retornaParametros(char *entrada);

int pesquisaNx(arquivo arq, int *nx);

int contaLinhas(arquivo arq);

char *removeExtensao(char * nome);

char *colocaBarraFinal(char *linha);

int floatlen(float x);

float distancia(float x1, float y1, float x2, float y2);

figura procuraFigura(lista figuras, int id);

int pontoInterno(figura fig, float x1, float y1);

int sobrepoem(figura figura1, figura figura2);

char* sobrepoeRetangulo(figura figura1, figura figura2);

char *criaLinha(float x1, float y1, float x2, float y2);

void centroMassa(figura fig, float *x, float *y);

lista processaBB(lista figuras, char* cor);

void *percorreLista(lista list, enum tipo_operacao tipo, void * auxiliar);

char * todoTextoFunc(int total_parametros, char ** parametros);

void trocaString(char **str1, char *str2);

char *dadosQuadraTxt(quadra q);

char *dadosHidranteTxt(equipamento h);

char *dadosSemaforoTxt(equipamento s);

char *dadosRadioTxt(equipamento r);

char *procuraObj(lista quadrs, lista hidrs, lista sems, lista rads, char *id);

char *deleteObj(lista quadrs, lista hidrs, lista sems, lista rads, char *id);

char *cbqFunc(lista list, float x, float y, float r, char *cstrk);

int RectDentroCirculo(float r, float x, float y, figura rect);

int RectDentroCirculoHelper(float r, float xc, float yc, float x1, float y1);

int RectDentroCirculoL2(float r1, float x1, float y1, figura rect);

char *dqFunc(svg arq_svg, lista quadras, lista hidrantes, lista semaforos, lista radios, char *tipoL, char *id, float r);

int retornaDistanciaL1(float r1, float x1, float y1, float x2, float y2, float w2, float h2);

char *translocafunc(svg arq_svg, lista quadras, lista hidrantes, lista semaforos, lista radios, float x, float y, float w, float h, float dx, float dy);

char *fIFunction(svg arq_svg, void **vet_s, void **vet_h, int s_n, int h_n,float x, float y, int ns, float r);

char *fSFunction(svg arq_svg, void **vet_s, lista quadras, int s_n, int k, char *cep, char *face, float num);

char *fHFunction(svg arq_svg, void **vet_h, lista quadras, int h_n, int k, char *cep, char *face, float num);
#endif
