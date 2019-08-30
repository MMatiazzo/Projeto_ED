#include "listaed.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct data{
  int prox;
  int ante;
  int pos;
  void * item;
}data_t;


typedef struct lista{
  int livre;
  int prim;
  int ulti;
  int tamanho;
  int total;
  data_t *node;
}lista_t;



int getLivre(lista_t *l);


lista criaLista(int tamanho){
  int i;
  lista_t * this;
  this = (lista_t *) malloc(sizeof(lista_t));
  this->node = (data_t *) malloc(sizeof(data_t) * tamanho);
  this->prim = -1;
  this->livre = 0;
  this->ulti = -1;
  this->total = 0;
  this->tamanho = tamanho;

  for(i = 0; i < tamanho; i++){
    this->node[i].prox = -1;
    this->node[i].ante = -1;
    this->node[i].item = NULL;
    this->node[i].pos = i;
  }

  return (lista) this;
}


lista insere_lista(lista list, item item){
  int i,k;
  lista_t * this;
  this = (lista_t *) list;
  if(this->livre == -1){
    #ifdef _DEBUG_
      printf("LISTA CHEIA\n");
    #endif
    return NULL;
  }

  this->total++;

  if(this->prim == -1){
    this->node[0].item = item;
    this->node[0].prox = -1;
    this->node[0].ante = -1;
    this->prim = 0;
    this->ulti = 0;
    this->livre = 1;
    return (lista) this;
  }

  i = this->livre;
  k = this->ulti;

  this->node[i].item = item;
  this->node[i].ante = k;
  this->node[i].prox = -1;
  this->node[k].prox = i;
  this->ulti = i;
  this->livre = getLivre(this);
  return (lista) this;
}


node getInicioLista(lista list){
  lista_t *this;
  this = (lista_t *) list;

  if(this->prim == -1){
    return NULL;
  }else{
    return (node) &this->node[this->prim];
  }
}


int listaVazia(lista list){
  lista_t *this = (lista_t *) list;
  return (this->prim == -1) ? 1 : 0;
}


int getTamanho(lista l){
  lista_t *this = (lista_t *) l;
  return this->total;
}


item getItem(lista l, int i){
   lista_t *this;
   this = (lista_t *) l;
   return this->node[i].item;
}


item getItemProx(lista l, node *no){
  data_t *this_no;
  lista_t *this_lista;
  this_no = (data_t *) *no;
  this_lista = (lista_t *) l;

  if(this_no->prox == -1){
    *no = NULL;
  }else{
    *no = &this_lista->node[this_no->prox];
  }
  return this_no->item;
}


item remove_lista(lista list, int i){
  lista_t *this;
  int anterior, proximo;
  this = (lista_t *) list;

  if(i == this->ulti || i == this->prim){
    if(i == this->ulti){
      this->ulti = this->node[i].ante;
      this->node[this->ulti].prox = -1;
      this->total--;
    }

    if(i == this->prim){
      this->prim = this->node[i].prox;
      this->node[this->prim].ante = -1;
      this->total--;
    }

  }else{
    anterior = this->node[i].ante;
    proximo = this->node[i].prox;
    this->node[anterior].prox = proximo;
    this->node[proximo].ante = anterior;
    this->node[i].ante = -1;
    this->node[i].prox = -1;
    anterior = this->node[anterior].prox;
    this->total--;
  }
}


void printaLista(lista l){
  int i;
  lista_t *this;
  this = (lista_t *) l;
  printf("anterior  |   item   |   prox  \n" );
  for(i = 0; i < this->tamanho; i++){
    printf("    %d    |    %p    |    %d     \n", this->node[i].ante, this->node[i].item, this->node[i].prox);
  }
}


int temProximo(node no){
  return !!no;
}


int getPosic(node no){
  data_t *this_no = (data_t *) no;
  return this_no->pos;
}


int getLivre(lista_t *l){
  int i;
  for(i = 0; i < l->tamanho; i++){
    if((l->node[i].prox == -1) && (l->node[i].ante == -1))
      return i;
  }
  return -1;
}


void apagaLista(lista l){
  lista_t *this = (lista_t *)l;
  free(this->node);
  free(this);
}

void **toVect(lista l){
  lista_t *this;
  this = (lista_t *) l;
  void **vet;
  vet = malloc(sizeof(void *) * this->total);
  int atual = this->prim;

  for(int i=0; i < this->total; i++){
    vet[i] = this->node[atual].item;
    atual = this->node[atual].prox;
  }
  return vet;
}
