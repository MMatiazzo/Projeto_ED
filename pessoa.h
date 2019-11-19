#ifndef __PESSOA_H_
#define __PESSOA_H_

typedef void *pessoa, *moradia;

pessoa criaPessoa(char *cpf, char *nome, char *sobrenome, char sexo, char *nascimento);

char *getPessoaCpf(pessoa p);

char *getPessoaNome(pessoa p);

char *getPessoaSobrenome(pessoa p);

char getPessoaSexo(pessoa p);

char *getPessoaNascimento(pessoa p);

void apagaPessoa(pessoa p);

moradia criaMoradia(char *cpf, char *cep, char face, int num, char *compl);char *getMoradiaCpf(moradia m);

char *getMoradiaCpf(moradia m);

char *getMoradiaCep(moradia m);

char getMoradiaFace(moradia m);

int getMoradiaNum(moradia m);

char *getMoradiaComplemento(moradia m);

void alteraMorador(moradia m, char *cep, char face, int num, char *compl);

void apagaMoradia(moradia m);

#endif