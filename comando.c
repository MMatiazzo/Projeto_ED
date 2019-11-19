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
  FOCO_INCENDIO,        "fi",
  FOCO_HIDRANTE,        "fh",
  FOCO_SEMAFORO,        "fs",
  IMPRIME_MORADOR,      "dm?",
  IMPRIME_ESTAB_MOR,    "de?",
  MUDAR_ENDERECO,       "mud",
  LISTAR_MORADORES,     "m?",
  PRINTA_ARVORE,        "dmprbt", 
  PONTO_LUZ,            "brl",
  POLIGONO_BORDA,       "mplg?",
  FINALIZA,             "#"
};



const struct{
  enum tipo_comando tipo;
  char * cmd;
}comando_exemplo_ec[]=
{
  DEFINE_ESTAB,         "t",
  INSERE_ESTAB,         "e"
};


const struct{
  enum tipo_comando tipo;
  char * cmd;
}comando_exemplo_pm[]=
{
  CADASTRA_PESSOA,      "p",
  ENDERECO_PESSOA,      "m"
};



const struct{
  enum tipo_comando tipo;
  char * cmd;
}comando_exemplo_it[]=
{
  PRINTA_ARVORE,        "dmprbt", 
  NAVEGA_IT,            "nav",
  QRY_IT,               "q",
  SAIR_IT,              "sai",
};


struct comando{
  enum tipo_comando tipo;
  int numero_parametros;
  char **parametros;
};


comando criaComando(char * entrada){ 
  struct comando *this;
  char *codigo, *aux;
  int i;

  this = (struct comando *) malloc(sizeof(struct comando)); 
  aux = (char *) malloc(sizeof(char) * strlen(entrada) + 1);

  this->tipo = NONE;
  this->parametros = NULL;
  strcpy(aux, entrada);
  if (strlen(aux) > 1)
    aux = strtok(aux, " ");

  for(i = 0; i < 34; i++){

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




comando criaComandoPm(char * entrada){ 
  struct comando *this;
  char *codigo, *aux;
  int i;

  this = (struct comando *) malloc(sizeof(struct comando));  
  aux = (char *) malloc(sizeof(char) * strlen(entrada) + 1);
  this->tipo = NONE;
  strcpy(aux, entrada);
  if (strlen(aux) > 1)
    aux = strtok(aux, " ");

  for(i = 0; i < 2; i++){

      if(!strcmp(aux, comando_exemplo_pm[i].cmd)){
        this->tipo = comando_exemplo_pm[i].tipo;
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


comando criaComandoEc(char * entrada){  
  struct comando *this;
  char *codigo, *aux;
  int i;

  this = (struct comando *) malloc(sizeof(struct comando));   
  aux = (char *) malloc(sizeof(char) * strlen(entrada) + 1);

  this->tipo = NONE;
  strcpy(aux, entrada);
  if (strlen(aux) > 1)
    aux = strtok(aux, " ");

  for(i = 0; i < 2; i++){

      if(!strcmp(aux, comando_exemplo_ec[i].cmd)){
        this->tipo = comando_exemplo_ec[i].tipo;
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



comando criaComandoIt(char * entrada){ 
  struct comando *this;
  char *codigo, *aux;
  int i;

  this = (struct comando *) malloc(sizeof(struct comando));  
  aux = (char *) malloc(sizeof(char) * strlen(entrada) + 1);

  this->tipo = NONE;
  strcpy(aux, entrada);
  if (strlen(aux) > 1)
    aux = strtok(aux, " ");

  for(i = 0; i < 4; i++){

      if(!strcmp(aux, comando_exemplo_it[i].cmd)){
        this->tipo = comando_exemplo_it[i].tipo;
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
  if(!cmd)
    return NULL;
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
