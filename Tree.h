#ifndef __TREE_H_
#define __TREE_H_
#define BLACK 0
#define RED 1

#include <stdio.h>
#include <stdlib.h>

#define VERMELHO 1
#define PRETO 0

typedef void *arvore;
typedef void *item;
typedef void *node;

arvore criaArvore(int (*compare)(void*, void*));

void insertArvore(arvore T, item z);

void percorreArvore(arvore a, void (*funcao)(void *, void *), void *auxiliar);

int getArvoreTamanho(arvore a);

item getNodeData(node no);

node getNodeLeft(node no);

node getNodeRight(node no);

node getNodeParent(node no);

int getNodeCor(node no);

int isNulo(arvore a, node no);

node getArvoreRaiz(arvore a);

void **arvoreToArray(arvore a);

void deletaArvore(void * T, char* key, char* (*getId)(void*));

void arqpercorreArvoreDmAux(void *a,void (*printaSvg)(void*,void*,FILE*,int,int,char,int),FILE *arquivoSVG);

void percorreToPoligonAux(void *a,int (*dadosNode)(void*,void**,int,void*,void*,void*),void** vetPontos,int tam,void *arquivoSVG);

#endif