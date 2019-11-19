#include "pessoa.h"
#include <string.h>
#include <stdlib.h>


typedef struct{
    char *cpf;
    char *nome;
    char *sobrenome;
    char sexo;
    char *nascimento;
}pessoa_t;

typedef struct{
    char *cpf;
    char *cep;
    char face;
    int num;
    char *compl;
}moradia_t;

pessoa criaPessoa(char *cpf, char *nome, char *sobrenome, char sexo, char *nascimento){
    pessoa_t *this = malloc(sizeof(pessoa_t));
    this->cpf = malloc(sizeof(char)*(strlen(cpf)+1));
    strcpy(this->cpf,cpf);
    this->nome = malloc(sizeof(char)*(strlen(nome)+1));
    strcpy(this->nome, nome);
    this->sobrenome = malloc(sizeof(char)*(strlen(sobrenome)+1));
    strcpy(this->sobrenome, sobrenome);
    this->sexo = sexo;
    this->nascimento = malloc(sizeof(char)*(strlen(nascimento)+1));
    strcpy(this->nascimento, nascimento);
    return this;
}

char *getPessoaCpf(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    return this->cpf;
}

char *getPessoaNome(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    return this->nome;
}

char *getPessoaSobrenome(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    return this->sobrenome;
}

char getPessoaSexo(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    return this->sexo;
}

char *getPessoaNascimento(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    return this->nascimento;
}

void apagaPessoa(pessoa p){
    pessoa_t *this = (pessoa_t*) p;
    free(this->cpf);
    free(this->nome);
    free(this->sobrenome);
    free(this->nascimento);
    free(this);
}

moradia criaMoradia(char *cpf, char *cep, char face, int num, char *compl){
    moradia_t *this = malloc(sizeof(moradia_t));
    this->cpf = malloc(sizeof(char)*(strlen(cpf)+1));
    strcpy(this->cpf,cpf);
    this->cep = malloc(sizeof(char)*(strlen(cep)+1));
    strcpy(this->cep,cep);
    this->face = face;
    this->num = num;
    this->compl = malloc(sizeof(char)*(strlen(compl)+1));
    strcpy(this->compl,compl);
    return this;
}

char *getMoradiaCpf(moradia m){
    moradia_t *this = (moradia_t*) m;
    return this->cpf;
}

char *getMoradiaCep(moradia m){
    moradia_t *this = (moradia_t*) m;
    return this->cep;
}

char getMoradiaFace(moradia m){
    moradia_t *this = (moradia_t*) m;
    return this->face;
}

int getMoradiaNum(moradia m){
    moradia_t *this = (moradia_t*) m;
    return this->num;
}

char *getMoradiaComplemento(moradia m){
    moradia_t *this = (moradia_t*) m;
    return this->compl;
}

void alteraMorador(moradia m, char *cep, char face, int num, char *compl){
    moradia_t *this = (moradia_t*) m;
    free(this->cep);
    this->cep = malloc(sizeof(char)*strlen(cep)+1);
    strcpy(this->cep,cep);
    this->face = face;
    this->num = num;
    free(this->compl);
    this->compl = malloc(sizeof(char)*strlen(compl)+1); 
    strcpy(this->compl,compl);
}

void apagaMoradia(moradia m){
    moradia_t *this = (moradia_t*) m;
    free(this->cpf);
    free(this->cep);
    free(this->compl);
    free(this);
}