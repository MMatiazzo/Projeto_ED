#ifndef _ARQUIVO_H
#define _ARQUIVO_H

#include <stdio.h>
//#include "listaed.h"


typedef void * arquivo;

enum tipo_arquivo{LEITURA, ESCRITA, ALTERACAO};

arquivo abreArquivo(char *caminho, enum tipo_arquivo modo);

char * lerLinha(arquivo arq);

//lista recebeLinhas(arquivo arq, int total_linhas);

void escreveLinha(arquivo arq, char * entrada, ...);

void resetaArquivo(arquivo arq);

void fechaArquivo(arquivo arq);


#endif
