#ifndef __ESTABELECIMENTO_H_
#define __ESTABELECIMENTO_H_


typedef void* definicao, *estabelecimento;

void* criaDefinicao(char *codigo, char *descricao);

estabelecimento criaEstabelecimento(char *cnpj, char* cpf, char *codigo, char *cep, char face, int num, char *nome, void* q);

void insereEstab_xy(void *hash_quadras, estabelecimento estab);

int comparator_estabelecimento(estabelecimento e1, estabelecimento e2);

char* getEstabCnpj(void* estab);

char* getEstabCpf(void* estab);

char* getEstabCodigo(void* estab);

char* getEstabCep(void* estab);

char getEstabFace(void* estab);

int getEstabNum(void* estab);

char* getEstabNome(void* estab);

char *getDefinicaoTipo(void* estab);

float getEstabX(void* estab);

float getEstabY(void* estab);

void apagaDefinicao(void* estab);

void apagaEstab(estabelecimento estab);

#endif