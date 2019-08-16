#ifndef _LISTA_E_D_
#define _LISTA_E_D_

typedef void* lista;
typedef void* item;
typedef void* node;

lista criaLista(int tamanho);

lista insere_lista(lista list, void* item);

node getInicioLista(lista list);

int listaVazia(lista list);

int getTamanho(lista l);

item getItem(lista l, int i);

item getItemProx(lista l, node *no);

int temProximo(node no);

int getPosic(node no);

item remove_lista(lista list, int i);

void apagaLista(lista list);

void printaLista(lista l);

void apagaLista(lista l);

#endif
