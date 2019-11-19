#ifndef _FUNCOES_H_
#define _FUNCOES_H_

#include "PontoBrl.h"
#include "Tree.h"
#include "hash.h"
#include "figura.h"
#include "equipamento.h"
#include "quadra.h"
//#include "listaed.h"
#include "arquivo.h"
#include "svg.h"
#include <limits.h>
#include <stdbool.h>

#define PI 3.1415

enum tipo_operacao{
  EXECUTA_COMANDO,
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

//figura procuraFigura(lista figuras, int id);

int pontoInterno(figura fig, float x1, float y1);

int sobrepoem(figura figura1, figura figura2);

char* sobrepoeRetangulo(figura figura1, figura figura2);

char *criaLinha(float x1, float y1, float x2, float y2);

void centroMassa(figura fig, float *x, float *y);

void** processaBB(arvore figuras, char* cor);

//void *percorreLista(lista list, enum tipo_operacao tipo, void * auxiliar);

char * todoTextoFunc(int total_parametros, char ** parametros);

void trocaString(char **str1, char *str2);

char *dadosQuadraTxt(quadra q);

char *dadosHidranteTxt(equipamento h);

char *dadosSemaforoTxt(equipamento s);

char *dadosRadioTxt(equipamento r);

char *procuraObj(hash_table quadrs, hash_table hidrs, hash_table sems, hash_table rads, char *id);

char *deleteObj(arvore quadrs, arvore hidrs, arvore sems, arvore rads, hash_table quadras_hash, hash_table hidrante_hash, hash_table semaforos_hash, hash_table radios_hash, char *id);

void cbqFunc(arvore a, float x, float y, float r, char *cstrk, arquivo txt);

void RectDentroCirculo(void *auxiliar, quadra quadra_atual);

int comparatorFoco(equipamento eq1, equipamento eq2, float x, float y);

int RectDentroCirculoHelper(float r, float xc, float yc, float x1, float y1);

void RectDentroCirculoL2(void *auxiliar, quadra quadra_atual);

void dqFunc(svg arq_svg, arvore quadras,hash_table quadras_hash, hash_table hidrantes, hash_table semaforos, hash_table radios, char *tipoL, char *id, float r, arquivo txt);

int retornaDistanciaL1(float r1, float x1, float y1, float x2, float y2, float w2, float h2);

char *translocafunc(svg arq_svg, arvore quadras, arvore hidrantes, arvore semaforos, arvore radios, float x, float y, float w, float h, float dx, float dy);

char *fIFunction(svg arq_svg, void **vet_s, void **vet_h, int s_n, int h_n,float x, float y, int ns, float r);

char *fSFunction(svg arq_svg, void **vet_s, arvore quadras, int s_n, int k, char *cep, char *face, float num);

char *fHFunction(svg arq_svg, void **vet_h, arvore quadras, int h_n, int k, char *cep, char *face, float num);

void imprimeMoradores(arvore a, arquivo txt, char *cep, hash_table pessoas);

int navegaArvore(arvore a, node ramo, char*(*toString)(void *));

void printTree(arvore a, svg arq_svg);

//void comandoBrl(arvore predios, arvore muros,arvore quadras, arquivo svg, float xFoco, float yFoco);

//novo
/*
double calcularAngulo(Ponto p, double x, double y);
double distanciaL1(double x1, double y1, double x2, double y2);
void Cidade_processarBombaRaioLuminoso(arvore predios, arvore muros, double x, double y, void *svg, bool brl, FILE* txt, FILE* arq);
*/

#endif
