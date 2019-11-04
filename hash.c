#include "hash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int size;
    void **data;
    char* (*getId)(void *);
}hash_t;
 
 
typedef struct node{
    void *data;
    struct node *prox;
}node;


typedef struct teste{
    char nome[20];
    int n;
}teste_t;
 
int existeChave(hash_table t, char *chave);
  
hash_table criaHashTable(int n, char *(*getId)(void *)){
    hash_t *this;
    this = malloc(sizeof(hash_t));
    this->data = malloc(sizeof(void *) * n);
    this->getId = getId;
    for(int i = 0; i < n; i++){
        this->data[i] = NULL;
    }
    this->size = n;
    return this;
}
 
 
 
void insereHashTable(hash_table t, void *item){
    hash_t *this = (hash_t *) t;
    char *chave = this->getId(item);
    node *this_node;
    int k;
 
    if(existeChave(t, chave))
        return;
 
    k = hashFunction(chave, this->size);
    if(this->data[k] == NULL){
        this_node = malloc(sizeof(node));
        this->data[k] = this_node;
    }else{
        this_node = this->data[k];
        while(this_node->prox != NULL){
            this_node = this_node->prox;
        }
        this_node->prox = malloc(sizeof(node));
        this_node = this_node->prox;
    }
    this_node->data = item;
    this_node->prox = NULL;
}
 
void remove_hash(hash_table t, char *chave){
    hash_t *this = (hash_t*) t;
    node *node_atual;
    node *node_anterior = NULL;
    int key;
    if(!existeChave(t, chave))
        return;
    key = hashFunction(chave, this->size);
    node_atual = this->data[key];
    while(node_atual != NULL){
        if(!strcmp(this->getId(node_atual->data),chave)){
            if(node_anterior == NULL){
                this->data[key] = node_atual->prox;
            }else{
                node_anterior->prox = node_atual->prox;
            }
            free(node_atual);
            break;
        }
        node_anterior = node_atual;
        node_atual = node_atual->prox;
    }
}
 
void *getHashTable(hash_table t, char *chave){
    hash_t *this = (hash_t*) t;
    node *this_node;
    int key;
    if(!existeChave(t, chave))
        return NULL;
 
    key = hashFunction(chave, this->size);
    if(!this->data[key])
        return NULL;
    this_node = this->data[key];
    while(this_node){
        if(!strcmp(this->getId(this_node->data), chave)){
            return this_node->data;
        }
        this_node = this_node->prox;
    }
    return NULL;
}
 
 
int existeChave(hash_table t, char *chave){
    hash_t *this = (hash_t *) t;
    node *this_node;
    int key = hashFunction(chave, this->size);
    if(!this->data[key])
        return 0;
    this_node = this->data[key];
    while(this_node){
        if(!strcmp(this->getId(this_node->data), chave))
            return 1;
        this_node = this_node->prox;
    }
    
    return 0;
}

int hashFunction(char *chave, int size){
    int p[50] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229};
    int k = 0;
 
    for(int i = 0; i < strlen(chave); i++){
        k += chave[i] * p[i];
    }
    k %= size;
    return k;
}