#include "comando.h"
#include "funcoes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


const struct{
  enum tipo_comando tipo;
  char * cmd;
}comando_exemplo[]=
{
  ALTERA_MAXIMOS,       "nx",
  CRIA_CIRCULO,         "c",
  CRIA_RETANGULO,       "r",
  TODO_TEXTO,           "t",
  SOBREPOSICAO,         "o?",
  PONTO_INTERNO,        "i?",
  DISTANCIA_CENTRO,     "d?",
  CRIA_BOUNDING_BOX,    "bb",
  INSERE_QUADRA,        "q",
  INSERE_HIDRANTE,      "h",
  INSERE_SEMAFORO,      "s",
  INSERE_RADIO,         "rb",
  MUDA_PADRAO_QUADRA,   "cq",
  MUDA_PADRAO_HIDRANTE, "ch",
  MUDA_PADRAO_RADIO,    "cr",
  MUDA_PADRAO_SEMAFORO, "cs",
  ESPESSURA_FIGURAS,    "sw",
  DELETA_OBJETOS,       "del",
  COORDENADAS_ESPECIE,  "crd?",
  COR_BORDA_QUADRA,     "cbq",
  DQ_REMOVE_QUADRA,     "dq",
  TRANSLOCA,            "trns",
  CRIA_PREDIO,          "prd",
  CRIA_MURO,            "mur",
  FINALIZA,             "#"
};





struct comando{
  enum tipo_comando tipo;
  int numero_parametros;
  char **parametros;
};






comando criaComando(char * entrada){     //TEM QUE DAR FREE NOS PARAMETROS
  struct comando *this;
  char *codigo, *aux;
  int i;

  this = (struct comando *) malloc(sizeof(struct comando));       //TEM QUE DAR FREE
  aux = (char *) malloc(sizeof(char) * strlen(entrada) + 1);
  // printf("LINHA COMANDO:%s\n", entrada );
  this->tipo = NONE;
  strcpy(aux, entrada);
  if (strlen(aux) > 1)
    aux = strtok(aux, " ");

  for(i = 0; i < 25; i++){

      if(!strcmp(aux, comando_exemplo[i].cmd)){
        this->tipo = comando_exemplo[i].tipo;
        this->numero_parametros = contaParametros(entrada);
        this->parametros = retornaParametros(entrada);
      }
  }

  free(aux);
  if(this->tipo == NONE){
    free(this);
    return NULL;
  }else{
    return (comando) this;
  }
}


char **getComandoParametros(comando cmd){
  struct comando * this;
  this = (struct comando *) cmd;
  return this->parametros;
}


enum tipo_comando getTipoComando(comando cmd){
  struct comando * this;
  this = (struct comando *)cmd;
  return this->tipo;
}


int getNumeroParametros(comando cmd){
  struct comando *this;
  this = (struct comando *) cmd;
  return this->numero_parametros;
}


void apagaComandoGeo(comando cmd){
  if(!cmd) return;

  struct comando *this;
  this = (struct comando *) cmd;
  int i = 0;

  while(i < this->numero_parametros){
    free(this->parametros[i]);
    i++;
  }
  free(this->parametros);
  free(this);
}
