#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "arquivo.h"

struct arquivo{
  FILE *arq;
  enum tipo_arquivo modo;
  char *path;
};



arquivo abreArquivo(char *caminho, enum tipo_arquivo modo){
  struct arquivo *this;
  this = (struct arquivo *) malloc(sizeof(struct arquivo));
  switch (modo) {
    case LEITURA: this->arq = fopen(caminho, "r"); break;
    case ESCRITA: this->arq = fopen(caminho, "w"); break;
    case ALTERACAO: this->arq = fopen(caminho, "a"); break;
  }
  if(!this->arq){
    free(this);
    return NULL;
  }
  this->modo = modo;
  this->path = (char *) malloc(sizeof(char) * strlen(caminho) + 1);
  strcpy(this->path, caminho);
  return (void *) this;
}


char * lerLinha(arquivo arq){
  struct arquivo *this;
  char* linha = NULL, linhaaux[500];
  this = (struct arquivo *) arq;

  if(fgets(linhaaux, 500, this->arq)){
    linha = (char *) malloc(sizeof(char) * strlen(linhaaux) + 1);   //TEM QUE DAR FREE
    strcpy(linha, linhaaux);
    linha[strlen(linha) - 1] = '\0';
  }

  return linha;
}


void escreveLinha(arquivo arq, char * entrada, ...){
  struct arquivo *this;
  this = (struct arquivo *) arq;
  if(this->modo == LEITURA)
    return;

  va_list ap;
  va_start(ap, entrada);
  vfprintf(this->arq, entrada, ap);
  va_end(ap);
  // fputs(entrada, this->arq);
  fputc('\n', this->arq);
}


void resetaArquivo(arquivo arq){
  struct arquivo *this;
  this = (struct arquivo *) arq;
  fseek(this->arq, 0, SEEK_SET);
}


void fechaArquivo(arquivo arq){
  struct arquivo *this;
  this = (struct arquivo *)arq;
  fclose(this->arq);
  free(this->path);
  free(this);
}


// lista getLinhasLista(lista l, )
