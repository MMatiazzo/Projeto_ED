#ifndef __TREE_H_
#define __TREE_H_
#define BLACK 0
#define RED 1

typedef void *arvore;
typedef void *item;

arvore criaArvore(int (*compare)(void*, void*));

void insertArvore(arvore T, item z);

void percorreArvore(arvore a, void (*funcao)(void *, void *), void *auxiliar);

int getArvoreTamanho(arvore a);

void **arvoreToArray(arvore a);

void deletaArvore(void * T, char* key, char* (*getId)(void*));


#endif