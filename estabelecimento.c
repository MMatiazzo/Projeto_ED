#include "estabelecimento.h"
#include "quadra.h"
#include "hash.h"
#include <string.h>
#include <stdlib.h>

typedef struct{
    char *tipo;
    char *descricao;
}defin_t;


typedef struct{
    char *cnpj, *cpf, *codigo, *cep, face, *nome;
    int num;
    float x, y;
}estab_t;



void* criaDefinicao(char *codigo, char *descricao){
    defin_t *this = malloc(sizeof(defin_t));
    this->tipo = malloc(sizeof(char)*(strlen(codigo)+1));
    strcpy(this->tipo,codigo);
    this->descricao = malloc(sizeof(char)*(strlen(descricao)+1));
    strcpy(this->descricao,descricao);
    return this;
}



char *getDefinicaoTipo(void* estab){
    defin_t *this = (defin_t*) estab;
    return this->tipo;
}


void apagaDefinicao(void* estab){
    defin_t *this = (defin_t*) estab;
    free(this->descricao);
    free(this->tipo);
    free(this);
}




estabelecimento criaEstabelecimento(char *cnpj, char* cpf, char *codigo, char *cep, char face, int num, char *nome, quadra q){
    estab_t *this = malloc(sizeof(estab_t));
    this->cnpj = malloc(sizeof(char) * (strlen(cnpj)+1));
    strcpy(this->cnpj,cnpj);
    this->cpf = malloc(sizeof(char)*(strlen(cpf)+1));
    strcpy(this->cpf,cpf);
    this->codigo = malloc(sizeof(char)*(strlen(codigo)+1));
    strcpy(this->codigo,codigo);
    this->cep = malloc(sizeof(char)*(strlen(cep)+1));
    strcpy(this->cep,cep);
    this->face = face;
    this->num = num;
    this->nome = malloc(sizeof(char)*(strlen(nome)+1));
    strcpy(this->nome,nome);

        
    if(this->face == 'N'){
        this->x = getXfig(getQuadraRect(q)) + this->num;
        this->y = getYfig(getQuadraRect(q)) + getH(getQuadraRect(q));
    }else if(this->face == 'S'){
        this->x = getXfig(getQuadraRect(q)) + this->num;
        this->y = getYfig(getQuadraRect(q));
    }else if(this->face == 'O'){
        this->x = getXfig(getQuadraRect(q)) + getW(getQuadraRect(q));
        this->y = getYfig(getQuadraRect(q)) + this->num;
    }else{
        this->x = getXfig(getQuadraRect(q));
        this->y = getYfig(getQuadraRect(q)) + this->num;
    }



    return this;

}

int comparator_estabelecimento(estabelecimento e1, estabelecimento e2){
    estab_t *this = (estab_t*) e1;
    estab_t *this_2 = (estab_t*) e2;

    if(this->x < this_2->x){
        return -1;
    }else if(this->x > this_2->x){
        return 1;
    }else if(this->y < this_2->y){
        return -1;
    }else if(this->y > this_2->y){
        return 1;
    }
    return -1;
}

void insereEstab_xy(void *hash_quadras, estabelecimento estab){
    estab_t *this = (estab_t*) estab;
    
    quadra q = getHashTable(hash_quadras,this->cep);
    if(this->face == 'N'){
        this->x = getXfig(getQuadraRect(q)) + this->num;
        this->y = getYfig(getQuadraRect(q)) + getH(getQuadraRect(q));
    }else if(this->face == 'S'){
        this->x = getXfig(getQuadraRect(q)) + this->num;
        this->y = getYfig(getQuadraRect(q));
    }else if(this->face == 'O'){
        this->x = getXfig(getQuadraRect(q)) + getW(getQuadraRect(q));
        this->y = getYfig(getQuadraRect(q)) + this->num;
    }else{
        this->x = getXfig(getQuadraRect(q));
        this->y = getYfig(getQuadraRect(q)) + this->num;
    }
    
}

char* getEstabCnpj(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->cnpj;
}

char* getEstabCpf(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->cpf;
}

char* getEstabCodigo(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->codigo;
}

char* getEstabCep(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->cep;
}

char getEstabFace(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->face;
}

int getEstabNum(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->num;
}

char* getEstabNome(void* estab){
    estab_t *this = (estab_t*) estab;
    return this->nome;
}

float getEstabX(void* estab){
    estab_t *this = (estab_t *) estab;
    return this->x;
}

float getEstabY(void* estab){
    estab_t *this = (estab_t *) estab;
    return this->y;
}

void apagaEstab(estabelecimento estab){
    estab_t *this = (estab_t*) estab;
    free(this->cnpj);
    free(this->cep);
    free(this->codigo);
    free(this->cpf);
    free(this->nome);
    free(this);
}