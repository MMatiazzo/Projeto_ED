#include "brl.h"
#include "PontoBrl.h"
#include "Tree.h"
#include "hash.h"
#include "executor.h"
#include "estabelecimento.h"
#include "pessoa.h"
#include "figura.h"
#include "quadra.h"
#include "equipamento.h"
#include "predio.h"
#include "segmento.h"
#include "texto.h"
#include "funcoes.h"
#include "svg.h"
#include "arquivo.h"
#include "heapsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct executor{
  char* dir_base;
  char* nome_entrada;
  char* nome_consulta;
  char* dir_saida;
  char* dir_estab;
  char* dir_pessoas;
  int interativo;

  arvore figuras;
  arvore quadras;
  arvore hidrantes;
  arvore semaforos;
  arvore radios;
  arvore predios;
  arvore muros;
  arvore textos;
  arvore moradia;
  arvore estabelecimento;

  hash_table quadras_hash;
  hash_table figuras_hash;
  hash_table hidrantes_hash;
  hash_table semaforos_hash;
  hash_table radios_hash;
  hash_table pessoa_hash;
  hash_table definicao_hash;
  hash_table estabelecimento_hash;
  hash_table moradia_hash;

  svg     arq_svg;
  arquivo arq_txt;

  struct{
    int figuras,    total_figuras;
    int quadras,    total_quadras;
    int hidrantes,  total_hidrantes;
    int semaforos,  total_semaforos;
    int radios,     total_radios;
    int predios,    total_predios;
    int muros,      total_muros;
  }maximos;

  struct{
    char *circulo;
    char *retangulo;
  }espessuras;

  struct{

    struct {
      char *borda;
      char *fill;
      char *espessura;
    }quadra;

    struct {
      char *borda;
      char *fill;
      char *espessura;
    }hidrante;

    struct {
      char *borda;
      char *fill;
      char *espessura;
    }radio;

    struct {
      char *borda;
      char *fill;
      char *espessura;
    }semaforo;

  }cor;
};


char *getNomeBase(char* linha);


executor criaExecutor(){
  struct executor *this;
  this = (struct executor*) malloc(sizeof(struct executor));
  this->interativo = 0;
  this->arq_txt = NULL;
  this->nome_entrada  = NULL;
  this->nome_consulta = NULL;
  this->dir_saida     = NULL;
  this->dir_base      = (char *) malloc(3 * sizeof(char));
  strcpy(this->dir_base, "./");
  this->dir_pessoas = NULL;
  this->dir_estab = NULL;
  this->figuras        = criaArvore(comparatorFig);
  this->quadras        = criaArvore(comparatorQuadra);
  this->hidrantes      = criaArvore(equipamentoComparator);
  this->semaforos      = criaArvore(equipamentoComparator);
  this->radios         = criaArvore(equipamentoComparator);
  this->predios        = criaArvore(comparatorPredio);
  this->muros          = criaArvore(comparatorMuroAux);
  this->textos         = criaArvore(comparatorMuro);   //gambiarra
  this->moradia        = criaArvore(comparatorMuro); //gambiarra 2.0
  this->estabelecimento= criaArvore(comparator_estabelecimento);
  this->quadras_hash   = NULL;
  this->figuras_hash   = NULL;
  this->hidrantes_hash = NULL;
  this->semaforos_hash = NULL;
  this->radios_hash    = NULL;
  this->pessoa_hash = NULL;
  this->definicao_hash = NULL;
  this->estabelecimento_hash = NULL;
  this->moradia_hash = NULL;
  this->maximos.total_figuras   = 0;
  this->maximos.total_quadras   = 0;
  this->maximos.total_hidrantes = 0;
  this->maximos.total_semaforos = 0;
  this->maximos.total_radios    = 0;
  this->maximos.total_predios   = 0;
  this->maximos.total_muros     = 0;
  this->maximos.figuras   = 0;
  this->maximos.quadras   = 0;
  this->maximos.hidrantes = 0;
  this->maximos.semaforos = 0;
  this->maximos.radios    = 0;
  this->maximos.muros     = 0;
  this->maximos.predios   = 0;
  this->espessuras.circulo    = (char *) malloc(sizeof(char) * 2 ); strcpy(this->espessuras.circulo,      "1");
  this->espessuras.retangulo  = (char *) malloc(sizeof(char) * 2 ); strcpy(this->espessuras.retangulo,    "1");
  this->cor.quadra.borda      = (char *) malloc(sizeof(char) * 5 ); strcpy(this->cor.quadra.borda,        "blue");
  this->cor.quadra.fill       = (char *) malloc(sizeof(char) * 15); strcpy(this->cor.quadra.fill,         "cornflowerblue");
  this->cor.quadra.espessura  = (char *) malloc(sizeof(char) * 2 ); strcpy(this->cor.quadra.espessura,    "1");
  this->cor.hidrante.borda    = (char *) malloc(sizeof(char) * 10); strcpy(this->cor.hidrante.borda,      "chocolate");
  this->cor.hidrante.fill     = (char *) malloc(sizeof(char) * 6 ); strcpy(this->cor.hidrante.fill,       "brown");
  this->cor.hidrante.espessura= (char *) malloc(sizeof(char) * 2 ); strcpy(this->cor.hidrante.espessura,  "1");
  this->cor.radio.borda       = (char *) malloc(sizeof(char) * 8 ); strcpy(this->cor.radio.borda,         "dimgrey");
  this->cor.radio.fill        = (char *) malloc(sizeof(char) * 9 ); strcpy(this->cor.radio.fill,          "darkgrey");
  this->cor.radio.espessura   = (char *) malloc(sizeof(char) * 2 ); strcpy(this->cor.radio.espessura,     "1");
  this->cor.semaforo.borda    = (char *) malloc(sizeof(char) * 12); strcpy(this->cor.semaforo.borda,      "forestgreen");
  this->cor.semaforo.fill     = (char *) malloc(sizeof(char) * 12); strcpy(this->cor.semaforo.fill,       "greenyellow");
  this->cor.semaforo.espessura= (char *) malloc(sizeof(char) * 2 ); strcpy(this->cor.semaforo.espessura,  "1");
  return (void *) this;
}


executor setParametros(executor executor, int argc, char* argv[]){
  struct executor *this;
  int i;
  char *nome_base, *dirsaida;
  this = (struct executor *) executor;

  for(i = 1 ; i < argc ; i++){

    if(!strcmp("-e", argv[i])){
      free(this->dir_base);
      this->dir_base = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->dir_base, argv[i]);
    }

    if(!strcmp("-f", argv[i])){
      this->nome_entrada = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->nome_entrada, argv[i]);
    }

    if(!strcmp("-q" , argv[i])){
      this->nome_consulta = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->nome_consulta, argv[i]);
    }


    if(!strcmp("-o" , argv[i])){
      this->dir_saida = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->dir_saida, argv[i]);
    }

    if(!strcmp("-ec" , argv[i])){
      this->dir_estab = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->dir_estab, argv[i]);
    }

    if(!strcmp("-pm" , argv[i])){
      this->dir_pessoas = (char *) malloc(sizeof(char) * strlen(argv[++i]) + 1);
      strcpy(this->dir_pessoas, argv[i]);
    }

    if(!strcmp("-i" , argv[i])){
      this->interativo = 1;
    }
  }


  if(!this->nome_entrada){
    printf("Nome arquivo nao especificado. Finalizando programa\n");
    exit(EXIT_FAILURE);
  }

  if(!this->dir_saida){
    printf("Diretorio de saida nao especificado. Finalizando programa\n");
    exit(EXIT_FAILURE);
  }


  nome_base = getNomeBase(this->nome_entrada);
  dirsaida = colocaBarraFinal(this->dir_saida);
  char * path = (char *) malloc(sizeof(char) * strlen(nome_base) + strlen(dirsaida) + 5);
  sprintf(path, "%s%s.svg", dirsaida, nome_base);
  this->arq_svg = criaSVG(path);
  free(nome_base);
  free(dirsaida);
  free(path);

}


void setQry(executor exec, char *nome){
  struct executor *this = (struct executor *) exec;
  free(this->nome_consulta);
  this->nome_consulta = malloc(sizeof(char) * strlen(nome) + 1);
  strcpy(this->nome_consulta, nome); 
}


char* getParametros(executor executor, enum tipo_parametro tipo){
  struct executor *this;
  this = (struct executor *) executor;
  switch (tipo) {
    case NOME_ENTRADA: return this->nome_entrada;
    case NOME_CONSULTA: return this->nome_consulta;
    case DIR_BASE: return this->dir_base;
    case DIR_SAIDA: return this->dir_saida;
  }

}


executor setNx(executor exec, int nf, int nq, int nh, int ns, int nr, int np, int nm){
  struct executor *this;
  this = (struct executor *) exec;
  this->maximos.figuras     = nf;
  this->maximos.quadras     = nq;
  this->maximos.hidrantes   = nh;
  this->maximos.semaforos   = ns;
  this->maximos.radios      = nr;
  this->maximos.predios     = np;
  this->maximos.muros       = nm;
  this->figuras_hash   = criaHashTable(nf, getId);
  this->quadras_hash   = criaHashTable(nq, getCep);
  this->hidrantes_hash = criaHashTable(nh, getEquipamentoId);
  this->semaforos_hash = criaHashTable(ns, getEquipamentoId);
  this->radios_hash    = criaHashTable(nr, getEquipamentoId);
  this->definicao_hash = criaHashTable(50, getDefinicaoTipo);
  this->estabelecimento_hash = criaHashTable(50, getEstabCnpj);
  this->pessoa_hash    = criaHashTable(50, getPessoaCpf);
  this->moradia_hash   = criaHashTable(50, getMoradiaCpf);

  return (executor) this;
}


char* getCaminhoBase(executor executor){
  struct executor *this;
  char * caminho;
  this = (struct executor*) executor;
  caminho = (char *) malloc(sizeof(char) * (strlen(this->dir_base) + strlen(this->nome_entrada) + 2));   //TEM Q DAR FREE

  if(!strcmp(this->dir_base, "./") && this->nome_entrada[0] == '/'){
    strcpy(caminho, this->nome_entrada);

  }else if(strcmp(this->dir_base, "./") && this->dir_base[strlen(this->dir_base) - 1] == '/'){

    if(this->nome_entrada[0] == '/'){
      strcpy(caminho, this->dir_base);
      strcat(caminho, &this->nome_entrada[1]);
    }else{
      strcpy(caminho, this->dir_base);
      strcat(caminho, this->nome_entrada);
    }
  }else{
    strcpy(caminho, this->dir_base);
    strcat(caminho, "/");
    strcat(caminho, this->nome_entrada);
  }


}

char* getCaminhoBasePessoa(executor executor){
  struct executor *this;
  char * caminho = NULL;
  this = (struct executor*) executor;
  if(!this->dir_pessoas){
    return NULL;
  }
  caminho = (char *) malloc(sizeof(char) * (strlen(this->dir_base) + strlen(this->dir_pessoas) + 2));   //TEM Q DAR FREE

  if(!strcmp(this->dir_base, "./") && this->dir_pessoas[0] == '/'){
    strcpy(caminho, this->dir_pessoas);

  }else if(strcmp(this->dir_base, "./") && this->dir_base[strlen(this->dir_base) - 1] == '/'){

    if(this->dir_pessoas[0] == '/'){
      strcpy(caminho, this->dir_base);
      strcat(caminho, &this->dir_pessoas[1]);
    }else{
      strcpy(caminho, this->dir_base);
      strcat(caminho, this->dir_pessoas);
    }
  }else{
    strcpy(caminho, this->dir_base);
    strcat(caminho, "/");
    strcat(caminho, this->dir_pessoas);
  } 
  return caminho;

}


char* getCaminhoBaseEstab(executor executor){
  struct executor *this;
  char * caminho = NULL;
  this = (struct executor*) executor;
  if(!this->dir_estab){
    return NULL;
  }
  caminho = (char *) malloc(sizeof(char) * (strlen(this->dir_base) + strlen(this->dir_estab) + 2));   //TEM Q DAR FREE

  if(!strcmp(this->dir_base, "./") && this->dir_estab[0] == '/'){
    strcpy(caminho, this->dir_estab);

  }else if(strcmp(this->dir_base, "./") && this->dir_base[strlen(this->dir_base) - 1] == '/'){

    if(this->dir_pessoas[0] == '/'){
      strcpy(caminho, this->dir_base);
      strcat(caminho, &this->dir_estab[1]);
    }else{
      strcpy(caminho, this->dir_base);
      strcat(caminho, this->dir_estab);
    }
  }else{
    strcpy(caminho, this->dir_base);
    strcat(caminho, "/");
    strcat(caminho, this->dir_estab);
  } 
  return caminho;

}

char *getNomeBase(char* linha){
  char * caminho, *apenas_nome;
  int i, j;
  apenas_nome = removeExtensao(linha);
  caminho = (char *)malloc(sizeof(char) * strlen(apenas_nome) + 1);
  strcpy(caminho, apenas_nome);
  j = strlen(apenas_nome) - 1;
  for(i = -1; i < j; j--){
    if(apenas_nome[j] == '/'){
      free(caminho);
      caminho = (char *)malloc(sizeof(char) * strlen(&apenas_nome[j + 1]) + 1);
      strcpy(caminho, &apenas_nome[j+1]);
      break;
    }
  }
  free(apenas_nome);
  return caminho;

}


char* getCaminhoQRY(executor executor){
  struct executor *this;
  char * caminho;
  this = (struct executor*) executor;
  caminho = (char *) malloc(sizeof(char) * (strlen(this->dir_base) + strlen(this->nome_consulta) + 1));   //TEM Q DAR FREE

  if(!strcmp(this->dir_base, "./") && this->nome_consulta[0] == '/'){
    strcpy(caminho, this->nome_consulta);

  }else if(strcmp(this->dir_base, "./") && this->dir_base[strlen(this->dir_base) - 1] == '/'){

    if(this->nome_consulta[0] == '/'){
      strcpy(caminho, this->dir_base);
      strcat(caminho, &this->nome_consulta[1]);
    }else{
      strcpy(caminho, this->dir_base);
      strcat(caminho, this->nome_consulta);
    }
  }else{
    strcpy(caminho, this->dir_base);
    strcat(caminho, "/");
    strcat(caminho, this->nome_consulta);
  }
}


int executarComando(executor exec, comando cmd){
  struct executor * this;
  texto new_texto;
  this = (struct executor *) exec;
  enum tipo_comando tipo = getTipoComando(cmd);
  int i = 0,j = 0;
  int total_parametros;
  char **parametros;
  char *aux, *aux2;
  float x1, x2, y1, y2, dist;
  figura new_fig = NULL, new_fig2 = NULL;
  svg svg_bbox;

  parametros = getComandoParametros(cmd);
  total_parametros = getNumeroParametros(cmd);

  switch(tipo){


      case ALTERA_MAXIMOS:
        this->maximos.figuras   = atoi(parametros[0]);
        this->maximos.quadras   = atoi(parametros[1]);
        this->maximos.hidrantes = atoi(parametros[2]);
        this->maximos.semaforos = atoi(parametros[3]);
        this->maximos.radios    = atoi(parametros[4]);
      break;


      case CRIA_CIRCULO:
        if(this->maximos.total_figuras >= this->maximos.figuras) break;
        new_fig = criaCirculo(atoi(parametros[0]), atof(parametros[1]), atof(parametros[2]), atof(parametros[3]), parametros[4], parametros[5], this->espessuras.circulo);
        insertArvore(this->figuras, new_fig);
        insereHashTable(this->figuras_hash, new_fig);
        this->maximos.total_figuras++;
      break;


      case CRIA_RETANGULO:
        if(this->maximos.total_figuras >= this->maximos.figuras) break;
        new_fig = criaRetangulo(atoi(parametros[0]), atof(parametros[1]), atof(parametros[2]), atof(parametros[3]), atof(parametros[4]), parametros[5], parametros[6], this->espessuras.retangulo);
        insertArvore(this->figuras, new_fig);
        insereHashTable(this->figuras_hash, new_fig);
        this->maximos.total_figuras++;
      break;

      case TODO_TEXTO:

        aux = todoTextoFunc(total_parametros, parametros);
        new_texto = criaTexto(atof(parametros[0]), atof(parametros[1]), aux);
        insertArvore(this->textos, new_texto);
        escreveTexto(this->arq_svg, new_texto);
        free(aux);
      break;

      // FASE 2    -    FASE 2    -    FASE 2    -    FASE 2    -    FASE 2    -    FASE 2    -    FASE 2    -    FASE 2    -


      case MUDA_PADRAO_QUADRA:
        trocaString(&this->cor.quadra.fill,         parametros[0]);
        trocaString(&this->cor.quadra.borda,        parametros[1]);
        trocaString(&this->cor.quadra.espessura,    parametros[2]);
      break;


      case MUDA_PADRAO_HIDRANTE:
        trocaString(&this->cor.hidrante.fill,       parametros[0]);
        trocaString(&this->cor.hidrante.borda,      parametros[1]);
        trocaString(&this->cor.hidrante.espessura,  parametros[2]);
      break;


      case MUDA_PADRAO_RADIO:
        trocaString(&this->cor.radio.fill,          parametros[0]);
        trocaString(&this->cor.radio.borda,         parametros[1]);
        trocaString(&this->cor.radio.espessura,     parametros[2]);
      break;


      case MUDA_PADRAO_SEMAFORO:
        trocaString(&this->cor.semaforo.fill,       parametros[0]);
        trocaString(&this->cor.semaforo.borda,      parametros[1]);
        trocaString(&this->cor.semaforo.espessura,  parametros[2]);
      break;


      case ESPESSURA_FIGURAS:
        trocaString(&this->espessuras.circulo,      parametros[0]);
        trocaString(&this->espessuras.retangulo,    parametros[0]);
      break;


      case INSERE_QUADRA:
        if(this->maximos.total_quadras >= this->maximos.quadras) break;
        new_fig = criaQuadra(parametros[0], atof(parametros[1]), atof(parametros[2]), atof(parametros[3]), atof(parametros[4]), this->cor.quadra.borda, this->cor.quadra.fill, this->cor.quadra.espessura);
        insereHashTable(this->quadras_hash,new_fig);
        insertArvore(this->quadras, new_fig);
        this->maximos.total_quadras++;
      break;


      case INSERE_HIDRANTE:
        if(this->maximos.total_hidrantes >= this->maximos.hidrantes) break;
        new_fig = criaHidrante(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.hidrante.borda, this->cor.hidrante.fill, this->cor.hidrante.espessura);
        insertArvore(this->hidrantes, new_fig);
        insereHashTable(this->hidrantes_hash, new_fig);
        this->maximos.total_hidrantes++;
      break;


      case INSERE_SEMAFORO:
        if(this->maximos.total_semaforos >= this->maximos.semaforos) break;
        new_fig = criaSemaforo(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.semaforo.borda, this->cor.semaforo.fill, this->cor.semaforo.espessura);
        insertArvore(this->semaforos, new_fig);
        insereHashTable(this->semaforos_hash, new_fig);
        this->maximos.total_semaforos++;
      break;


      case INSERE_RADIO:
      if(this->maximos.total_radios >= this->maximos.radios) break;
      new_fig = criaRadio(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.radio.borda, this->cor.radio.fill, this->cor.radio.espessura);
      insertArvore(this->radios, new_fig);
      insereHashTable(this->radios_hash, new_fig);
      this->maximos.total_radios++;
      break;


      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3

      case CRIA_PREDIO:
      if(this->maximos.total_predios >= this->maximos.predios) break;
      if(!(new_fig2 = getHashTable(this->quadras_hash, parametros[0]))){
        break;
      }
      new_fig2 = getQuadraRect(new_fig2);
      new_fig = criaPredio(parametros[0], parametros[1], atof(parametros[2]), atof(parametros[3]), atof(parametros[4]), atof(parametros[5]) , new_fig2);
      insertArvore(this->predios, new_fig);
      this->maximos.total_predios++;
      break;


      case CRIA_MURO:
      if(this->maximos.total_muros >= this->maximos.muros) break;
      new_fig = criaSegmento(atof(parametros[0]), atof(parametros[1]), atof(parametros[2]), atof(parametros[3]));
      insertArvore(this->muros, new_fig);
      this->maximos.total_muros++;
      break;

      default: break;         //FIM DOS COMANDOS


  }
}

void executarComandoPm(executor exec, comando cmd){
  struct executor * this;
  this = (struct executor *) exec;
  enum tipo_comando tipo = getTipoComando(cmd);
  int total_parametros;
  char **parametros;
  void *aux;
  
  parametros = getComandoParametros(cmd);
  total_parametros = getNumeroParametros(cmd);

  switch (tipo)
  {
  case CADASTRA_PESSOA:
    aux = criaPessoa(parametros[0],parametros[1],parametros[2],parametros[3][0],parametros[4]);
    if(!insereHashTable(this->pessoa_hash,aux)){
      apagaPessoa(aux);
    }
    break;
  
  case ENDERECO_PESSOA:
  if(getHashTable(this->pessoa_hash,parametros[0])){
    aux = criaMoradia(parametros[0],parametros[1],parametros[2][0],atoi(parametros[3]),parametros[4]);
    if(!insereHashTable(this->moradia_hash,aux)){
      apagaMoradia(aux);
    }else{
      insertArvore(this->moradia,aux);
    }
  }
  break;
  }
}

void executarComandoEc(executor exec, comando cmd){
  struct executor * this;
  this = (struct executor *) exec;
  enum tipo_comando tipo = getTipoComando(cmd);
  int total_parametros;
  char **parametros;
  void *aux;
  
  parametros = getComandoParametros(cmd);
  total_parametros = getNumeroParametros(cmd);

  switch (tipo)
  {
  case DEFINE_ESTAB:
    aux = criaDefinicao(parametros[0], parametros[1]);
    if(!insereHashTable(this->definicao_hash, aux))
      apagaDefinicao(aux);
    break;
  
  case INSERE_ESTAB:
      if(getHashTable(this->definicao_hash,parametros[2])){
        aux = criaEstabelecimento(parametros[0],parametros[1],parametros[2],parametros[3],parametros[4][0],atoi(parametros[5]),parametros[6], getHashTable(this->quadras_hash,parametros[3]));
        if(!insereHashTable(this->estabelecimento_hash,aux)){
          apagaEstab(aux);
        }else{
          insertArvore(this->estabelecimento,aux);
        }
      }
  break;
  }
}


int executarComandoQry(executor exec, comando cmd){
  struct executor * this;
  texto new_texto;
  this = (struct executor *) exec;
  enum tipo_comando tipo = getTipoComando(cmd);
  int i = 0,j = 0;
  int total_parametros;
  char **parametros;
  char *aux, *aux2, *dir_saida;
  float x1, x2, y1, y2, dist;
  figura new_fig, new_fig2;
  void *auxajuda;
  svg svg_bbox;
  FILE *auxsvg;

  void **vet_s, **vet_h, **vet_q, **bbox, **vet_fig;
  char *aux_dir;


  parametros = getComandoParametros(cmd);
  total_parametros = getNumeroParametros(cmd);

  switch(tipo){
    case SOBREPOSICAO:
    new_fig = getHashTable(this->figuras_hash, parametros[0]);
    new_fig2 = getHashTable(this->figuras_hash, parametros[1]);

    if( !new_fig || !new_fig2){
      printf("Figura não encontrada\n");
      break;
    }
    aux = (char *) malloc(sizeof(char) * (strlen(parametros[0]) + (strlen(parametros[1])) + 21));
    sprintf(aux, "o? %s %s\n", parametros[0], parametros[1]);



    if(sobrepoem(new_fig, new_fig2)){
      strcat(aux, "SIM SOBREPOEM\n");

    }else{
      strcat(aux, "NAO SOBREPOEM\n");
    }

    aux2 = sobrepoeRetangulo(new_fig, new_fig2);
    
    escreveLinhaSVG(this->arq_svg, aux2);
    escreveLinha(this->arq_txt, aux);
    free(aux2);
    free(aux);
    break;




    case PONTO_INTERNO:
      new_fig = getHashTable(this->figuras_hash, parametros[0]);
      char *cor;
      aux = (char *) malloc(sizeof(char) * (strlen(parametros[0]) + (strlen(parametros[1])) + strlen(parametros[2]) + 19));
      cor = (char *) malloc(sizeof(char) * 6);
      sprintf(aux, "i? %s %s %s\n", parametros[0], parametros[1], parametros[2]);
      strcpy(cor, "red");

      if(!new_fig){
        free(aux);
        free(cor);
        break;
      }else{
        if(pontoInterno(new_fig, atof(parametros[1]), atof(parametros[2]))){
          strcat(aux, "SIM INTERNO\n");
          strcpy(cor, "green");

        }else{
          strcat(aux, "NAO INTERNO\n");
        }
      }
      centroMassa(new_fig, &x1, &y1);
      x2 = atof(parametros[1]);
      y2 = atof(parametros[2]);
      escreveLinha(this->arq_txt, aux);
      free(aux);
      new_fig = criaCirculo(-1, 5, atof(parametros[1]), atof(parametros[2]), "black", cor, "1");
      desenhaFigura(this->arq_svg, new_fig);
      aux = criaLinha(x1, y1, x2, y2);
      escreveLinhaSVG(this->arq_svg, aux);

      apagaFigura(new_fig);
      free(aux);
      free(cor);
      break;


    case DISTANCIA_CENTRO:
      new_fig = getHashTable(this->figuras_hash, parametros[0]);
      new_fig2 = getHashTable(this->figuras_hash, parametros[1]);
      texto text;
      aux = (char *) malloc(sizeof(char) * strlen(parametros[0]) + strlen(parametros[1]) + 6);
      sprintf(aux, "d? %s %s\n", parametros[0], parametros[1]);
      if(!new_fig || !new_fig2){
        free(aux);
        break;
      }
      centroMassa(new_fig, &x1, &y1);
      centroMassa(new_fig2, &x2, &y2);
      dist = distancia(x1, y1, x2, y2);
      aux2 = (char *) malloc(sizeof(char) * (strlen(aux) + floatlen(dist) + 2));
      sprintf(aux2, "%s%f\n", aux, dist);
      free(aux);
      aux = (char *)malloc(sizeof(char) * floatlen(dist) + 1);
      sprintf(aux, "%f", dist);
      escreveLinha(this->arq_txt, aux2);
      text = criaTexto(  ((x1 + x2) / 2),  ((y1 + y2) / 2), aux);
      free(aux2);
      free(aux);
      aux = criaLinha(x1, y1, x2, y2);
      escreveLinhaSVG(this->arq_svg, aux);
      escreveTexto(this->arq_svg, text);
      apagaTexto(text);
      free(aux);
      break;

    case CRIA_BOUNDING_BOX:
      bbox = processaBB(this->figuras, parametros[1]);
      aux2 = removeExtensao(this->nome_entrada);
      dir_saida = colocaBarraFinal(this->dir_saida);
      aux = (char *) malloc(sizeof(char) * (strlen(parametros[0]) + strlen(aux2) + strlen(dir_saida) + 6));
      sprintf(aux, "%s%s-%s.svg", dir_saida, aux2, parametros[0]);
      svg_bbox = criaSVG(aux);

      percorreArvore(this->figuras, desenhaFigura, svg_bbox);

      for(i = 0; i < getArvoreTamanho(this->figuras); i++){
        desenhaFigura(svg_bbox, bbox[i]);
        apagaFigura(bbox[i]);
      }
 
      free(bbox);
      free(aux);
      free(aux2);
      free(dir_saida);
      fechaSVG(svg_bbox);
    break;



    case DELETA_OBJETOS:
      aux = deleteObj(this->quadras, this->hidrantes, this->semaforos, this->radios,this->quadras_hash,this->hidrantes_hash,this->semaforos_hash,this->radios_hash, parametros[0]);
      if(aux){
        aux2 = (char *)malloc(sizeof(char) * strlen(aux) + 10);
        sprintf(aux2, "Removido %s", aux);
        escreveLinha(this->arq_txt, aux2);
        free(aux);
        free(aux2);
      }
    break;


    case COORDENADAS_ESPECIE:
      aux = procuraObj(this->quadras_hash, this->hidrantes_hash, this->semaforos_hash, this->radios_hash, parametros[0]);
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;


    case COR_BORDA_QUADRA:
      cbqFunc(this->quadras, atof(parametros[0]), atof(parametros[1]), atof(parametros[2]), parametros[3], this->arq_txt);
    break;

    case DQ_REMOVE_QUADRA:
      dqFunc(this->arq_svg, this->quadras,this->quadras_hash, this->hidrantes_hash, this->semaforos_hash, this->radios_hash, parametros[0], parametros[1], atof(parametros[2]), this->arq_txt);
    break;


    case TRANSLOCA:
      aux = translocafunc(this->arq_svg, this->quadras, this->hidrantes, this->semaforos, this->radios, atof(parametros[0]), atof(parametros[1]),atof(parametros[2]),atof(parametros[3]),atof(parametros[4]),atof(parametros[5]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;


    case FOCO_INCENDIO:
      vet_s = arvoreToArray(this->semaforos);
      vet_h = arvoreToArray(this->hidrantes);

      heapsort(vet_s,getArvoreTamanho(this->semaforos),comparatorFoco, atof(parametros[0]), atof(parametros[1]));
      heapsort(vet_h,getArvoreTamanho(this->hidrantes),comparatorFoco, atof(parametros[0]), atof(parametros[1]));
      aux = fIFunction(this->arq_svg, vet_s, vet_h, getArvoreTamanho(this->semaforos), getArvoreTamanho(this->hidrantes),atof(parametros[0]),atof(parametros[1]),atoi(parametros[2]),atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }

      free(vet_s);
      free(vet_h);
    break;

    case FOCO_SEMAFORO:
      vet_s = arvoreToArray(this->semaforos);
      aux = fSFunction(this->arq_svg, vet_s, this->quadras_hash, getArvoreTamanho(this->semaforos), atoi(parametros[0]), parametros[1], parametros[2], atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }

      free(vet_s);

    break;

    case FOCO_HIDRANTE:
      vet_h = arvoreToArray(this->hidrantes);
      aux = fHFunction(this->arq_svg, vet_h, this->quadras_hash, getArvoreTamanho(this->hidrantes), atoi(parametros[0]), parametros[1], parametros[2], atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
      free(vet_h);
    break;

    case IMPRIME_MORADOR:
      escreveLinha(this->arq_txt,"dm? %s",parametros[0]);
      auxajuda = getHashTable(this->pessoa_hash,parametros[0]);
      if(auxajuda != NULL){
        escreveLinha(this->arq_txt,"cpf = %s | nome = %s | sobrenome = %s | sexo = %c | nascimento = %s",getPessoaCpf(auxajuda),getPessoaNome(auxajuda),getPessoaSobrenome(auxajuda), getPessoaSexo(auxajuda), getPessoaNascimento(auxajuda));
      }
    break;

    case IMPRIME_ESTAB_MOR:
      escreveLinha(this->arq_txt,"de? %s",parametros[0]);

      auxajuda = getHashTable(this->estabelecimento_hash,parametros[0]);

      void *auxajuda2;
      if(auxajuda != NULL){

        escreveLinha(this->arq_txt,"ESTABELECIMENTO -> cnpj = %s | cpf = %s | codt = %s | cep = %s | face = %c | num = %d | nome = %s\n",getEstabCnpj(auxajuda),getEstabCpf(auxajuda),getEstabCodigo(auxajuda),getEstabCep(auxajuda),getEstabFace(auxajuda),getEstabNum(auxajuda), getEstabNome(auxajuda));
        auxajuda2 = getHashTable(this->pessoa_hash,getEstabCpf(auxajuda));
        escreveLinha(this->arq_txt,"PROPRIETARIO -> cpf = %s | nome = %s | sobrenome = %s | sexo = %c | nascimento = %s\n",getPessoaCpf(auxajuda2),getPessoaNome(auxajuda2),getPessoaSobrenome(auxajuda2), getPessoaSexo(auxajuda2),getPessoaNascimento(auxajuda2));
      }
    break;

    case MUDAR_ENDERECO:
    escreveLinha(this->arq_txt,"mud %s %s %s %s %s",parametros[0],parametros[1],parametros[2],parametros[3],parametros[4]);
    auxajuda = getHashTable(this->moradia_hash, parametros[0]);
    auxajuda2 = getHashTable(this->pessoa_hash, parametros[0]);
    
    
    if(auxajuda){
      escreveLinha(this->arq_txt,"PESSOA -> cpf = %s | nome = %s | sobrenome = %s | sexo = %c | nascimento = %s",getPessoaCpf(auxajuda2),getPessoaNome(auxajuda2),getPessoaSobrenome(auxajuda2), getPessoaSexo(auxajuda2),getPessoaNascimento(auxajuda2));
      escreveLinha(this->arq_txt,"DADOS MORADIA ANTERIOR -> cpf = %s | cep = %s | face = %c | num = %d | compl = %s",getMoradiaCpf(auxajuda),getMoradiaCep(auxajuda),getMoradiaFace(auxajuda),getMoradiaNum(auxajuda),getMoradiaComplemento(auxajuda));
      alteraMorador(auxajuda,parametros[1],parametros[2][0],atoi(parametros[3]),parametros[4]);
      escreveLinha(this->arq_txt,"DADOS MORADIA NOVO -> cpf = %s | cep = %s | face = %c | num = %d | compl = %s\n",getMoradiaCpf(auxajuda),getMoradiaCep(auxajuda),getMoradiaFace(auxajuda),getMoradiaNum(auxajuda),getMoradiaComplemento(auxajuda));

    }
    break;

    case LISTAR_MORADORES:
    escreveLinha(this->arq_txt,"m? %s",parametros[0]);
    if(getHashTable(this->quadras_hash,parametros[0])){
      imprimeMoradores(this->moradia,this->arq_txt,parametros[0],this->pessoa_hash);
    }else{
      escreveLinha(this->arq_txt,"QUADRA INEXISTENTE\n");
    }
    break;

    case PRINTA_ARVORE:

      aux_dir = malloc(sizeof(char)*strlen(this->dir_saida)+strlen(parametros[1])+6);
      strcpy(aux_dir,this->dir_saida);
      strcat(aux_dir,parametros[1]);

      auxsvg = fopen(aux_dir,"w");
    if(!auxsvg)
      printf("NAO ABRIU\n");
    else{
      if(auxsvg){
        fputs("<svg fill-opacity='0.5'><rect width = '100%' height = '100%' fill = 'white' fill-opacity = '1.0'/>\n",auxsvg);
        if(parametros[0][0] == 'q'){
          arqpercorreArvoreDmAux(this->quadras,printaSvgQuadDm,auxsvg);
        }else if(parametros[0][0] == 'h'){
          arqpercorreArvoreDmAux(this->hidrantes,printaSvgHidDm,auxsvg);  
        }else if(parametros[0][0] == 's'){
          arqpercorreArvoreDmAux(this->semaforos,printaSvgHidDm,auxsvg);
        }else if(parametros[0][0] == 't'){
          arqpercorreArvoreDmAux(this->radios,printaSvgHidDm,auxsvg);
        }else if(parametros[0][0] == 'p'){
          arqpercorreArvoreDmAux(this->predios,printaSvgPredioDm,auxsvg);
        }else if(parametros[0][0] == 'm'){
          arqpercorreArvoreDmAux(this->muros,printaSvgMuroDm,auxsvg);
        }
        fputs("</svg>",auxsvg);
        fclose(auxsvg);
      }
      else{
        printf("ARQUIVO NÃO ABERTO\n");
      }

    }
    break;

    case PONTO_LUZ:    
    brlProcedure(atof(parametros[0]),atof(parametros[1]),this->arq_svg,this->quadras,this->muros,this->predios,this->quadras_hash);
  
    break;

    case POLIGONO_BORDA:
      aux = malloc(sizeof(char)*(strlen(parametros[0])+strlen(this->dir_base)+1));
      char arq[100];char dirOO[100];char linhaPol[100];char pol[999999];char x[100],y[100];
      int i;
      void **VetPonto;int tamanho = 0;
            strcpy(aux,this->dir_base);
            strcat(aux,parametros[0]);
            printf("%s\n\n",aux);
            FILE* arqPoligono = fopen(aux,"r");
            if(!arqPoligono){
              perror("Erro -> \n");
            }
            strcpy(pol,"");
            while(fgets(linhaPol, 500, arqPoligono) != NULL){
                tamanho++;
                sscanf(linhaPol,"%s %s",x,y);
                strcat(pol,x);strcat(pol,",");strcat(pol,y);strcat(pol," ");
            }
            strcat(pol,"\0");
            escreveLinha(getSvgArq(this->arq_svg),"<polygon points= '%s'/>\n",pol);
            VetPonto = malloc(sizeof(void*)*tamanho);
            fseek(arqPoligono,0,SEEK_SET);
            i = 0;
            while(fgets(linhaPol, 500, arqPoligono) != NULL){
                sscanf(linhaPol,"%s %s",x,y);
                void* ponto = NewPoint(atof(x),atof(y));
                VetPonto[i] = ponto;
                i++;
            }
            fclose(arqPoligono);
            percorreToPoligonAux(this->predios,verificaDentroPredio,VetPonto,tamanho,this->arq_svg);
            percorreToPoligonAux(this->quadras,verificaDentroQuadra,VetPonto,tamanho,this->arq_svg);
          break;

  }
}



void executarComandoI(executor exec){
  struct executor * this = (struct executor *) exec;
  char entrada[50];
  comando cmd;
  arquivo arquivo_qry;
  char **parametros, *aux, *linha, *caminho, *nome_base, *nome_base_qry, *dir_saida;
  char *aux_dir;
  FILE *auxsvg;
  while(1){
    system("clear");
    printf("=-=-=-=-=-=-=-=-=-=-=- MODO INTERATIVO -=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    printf("nav -> NAVEGAR ENTRE UMA DAS ARVORES\n");
    printf("  s -> SEMAFORO\n");
    printf("  r -> RADIO\n");
    printf("  q -> QUADRAS\n");
    printf("  h -> HIDRANTE\n");
    printf("q -> LE E EXECUTA O ARQUIVO DE CONSULTA QRY\n");
    printf("dmprbt [arvore] [nome arquivo]\n");
    printf("sai -> SAI DO MODO INTERATIVO\n");
    printf("COMANDO-->: ");
    scanf(" %[^\n]", entrada);

    
    cmd = criaComandoIt(entrada);
    if(cmd){
      parametros = getComandoParametros(cmd);
      char key;
      switch (getTipoComando(cmd))
      {
      case NAVEGA_IT:
        if(parametros[0][0] == 'h'){
          navegaArvore(this->hidrantes, getArvoreRaiz(this->hidrantes), equipamentoToString);
        }else if(parametros[0][0] == 's'){
          navegaArvore(this->semaforos, getArvoreRaiz(this->semaforos), equipamentoToString);
        }else if(parametros[0][0] == 'q'){
          navegaArvore(this->quadras, getArvoreRaiz(this->quadras), quadraToString);
        }else if(parametros[0][0] == 'r'){
          navegaArvore(this->radios, getArvoreRaiz(this->radios), equipamentoToString);
        }
        break;

      case QRY_IT:
        setQry(exec, parametros[0]);
        caminho = getCaminhoQRY(exec);
        arquivo_qry = abreArquivo(caminho, LEITURA);
        free(caminho);
        if(!arquivo_qry){
          printf("ARQUIVO NAO ABERTO\n");

        }else{
          nome_base = getNomeBase(this->nome_entrada);
          nome_base_qry = getNomeBase(this->nome_consulta);
          dir_saida = colocaBarraFinal(this->dir_saida);
          char * path = (char *) malloc(sizeof(char) * strlen(nome_base) + strlen(nome_base_qry) + strlen(dir_saida) + 6);
          sprintf(path, "%s%s-%s.svg", dir_saida, nome_base, nome_base_qry);
          this->arq_svg = criaSVG(path);
          sprintf(path, "%s%s-%s.txt", dir_saida, nome_base, nome_base_qry);
          this->arq_txt = abreArquivo(path,ESCRITA);
          while(linha = lerLinha(arquivo_qry)){
            cmd = criaComando(linha);
            executarComandoQry(exec, cmd);
            apagaComandoGeo(cmd);
            free(linha);
          }
        finalizaQRY(exec);
        fechaArquivo(arquivo_qry);
        fechaArquivo(this->arq_txt);
        }
      break;

      case PRINTA_ARVORE:
        aux_dir = malloc(sizeof(char)*strlen(this->dir_saida)+strlen(parametros[1])+6);
        strcpy(aux_dir,this->dir_saida);
        strcat(aux_dir,parametros[1]);
        strcat(aux_dir,".svg");
        auxsvg = fopen(aux_dir,"w");
      if(!auxsvg)
        printf("NAO ABRIU\n");
      else{
        if(auxsvg){
          fputs("<svg fill-opacity='0.5'><rect width = '100%' height = '100%' fill = 'white' fill-opacity = '1.0'/>\n",auxsvg);
          if(parametros[0][0] == 'q'){
            arqpercorreArvoreDmAux(this->quadras,printaSvgQuadDm,auxsvg);
          }else if(parametros[0][0] == 'h'){
            arqpercorreArvoreDmAux(this->hidrantes,printaSvgHidDm,auxsvg);  
          }else if(parametros[0][0] == 's'){
            arqpercorreArvoreDmAux(this->semaforos,printaSvgHidDm,auxsvg);
          }else if(parametros[0][0] == 't'){
            arqpercorreArvoreDmAux(this->radios,printaSvgHidDm,auxsvg);
          }else if(parametros[0][0] == 'p'){
            arqpercorreArvoreDmAux(this->predios,printaSvgPredioDm,auxsvg);
          }else if(parametros[0][0] == 'm'){
            arqpercorreArvoreDmAux(this->muros,printaSvgMuroDm,auxsvg);
          }
          fputs("</svg>",auxsvg);
          fclose(auxsvg);
        }
        else{
          printf("ARQUIVO NÃO ABERTO\n");
        }
      }
      break;

      case SAIR_IT:
        return;
      break;

      default:
        return;
        break;
      }
    }
  }

}





void escreveTudoSVG(executor exec){
  struct executor *this;
  this = (struct executor *)exec;

  percorreArvore(this->quadras   ,desenhaQuadra     , this->arq_svg);
  percorreArvore(this->figuras   ,desenhaFigura     , this->arq_svg);
  percorreArvore(this->hidrantes ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->semaforos ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->radios    ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->predios   ,desenhaPredio     , this->arq_svg);
  percorreArvore(this->muros     ,desenhaLineSVG    , this->arq_svg);
  printf("TAMANHO: %d\n\n", getArvoreTamanho(this->estabelecimento));
  percorreArvore(this->estabelecimento,desenhaEstab , this->arq_svg);

  fechaSVG(this->arq_svg);

}


int temQRY(executor exec){
  struct executor * this;
  this = (struct executor *) exec;
  char *nome_base, *nome_base_qry, *dir_saida;
  if(!this->nome_consulta)
    return 0;
  nome_base = getNomeBase(this->nome_entrada);
  nome_base_qry = getNomeBase(this->nome_consulta);
  dir_saida = colocaBarraFinal(this->dir_saida);
  char * path = (char *) malloc(sizeof(char) * strlen(nome_base) + strlen(nome_base_qry) + strlen(dir_saida) + 6);
  sprintf(path, "%s%s-%s.svg", dir_saida, nome_base, nome_base_qry);
  this->arq_svg = criaSVG(path);
  sprintf(path, "%s%s-%s.txt", dir_saida, nome_base, nome_base_qry);
  this->arq_txt = abreArquivo(path, ESCRITA);
  free(path);
  free(dir_saida);
  free(nome_base);
  free(nome_base_qry);
  return !!this->nome_consulta;
}


int temInterativo(executor exec){
  return ((struct executor *)exec)->interativo;
}


void finalizaQRY(executor exec){
  struct executor *this;
  figura figura_atual;
  texto texto_atual;

  arquivo arquivo_txt;
  this = (struct executor *)exec;


  percorreArvore(this->quadras   ,desenhaQuadra     , this->arq_svg);
  percorreArvore(this->figuras   ,desenhaFigura     , this->arq_svg);
  percorreArvore(this->hidrantes ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->semaforos ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->radios    ,desenhaEquipamento, this->arq_svg);
  percorreArvore(this->predios   ,desenhaPredio     , this->arq_svg);
  percorreArvore(this->muros     ,desenhaLineSVG    , this->arq_svg);
  percorreArvore(this->estabelecimento,desenhaEstab, this->arq_svg);

  fechaSVG(this->arq_svg);


}


void apagaExecutor(executor executor){
  struct executor *this;
  this = (struct executor *) executor;

  if(this){

    if(this->nome_entrada){
      free(this->nome_entrada);
      this->nome_entrada = NULL;
    }
    if(this->dir_base){
      free(this->dir_base);
      this->dir_base = NULL;
    }
    if(this->nome_consulta){
      free(this->nome_consulta);
      this->nome_consulta = NULL;
    }
    if(this->dir_saida){
      free(this->dir_saida);
      this->dir_saida = NULL;
    }

    free(this->espessuras.circulo);
    free(this->espessuras.retangulo);
    free(this->cor.quadra.borda);
    free(this->cor.quadra.fill);
    free(this->cor.quadra.espessura);
    free(this->cor.hidrante.borda);
    free(this->cor.hidrante.fill);
    free(this->cor.hidrante.espessura);
    free(this->cor.radio.borda);
    free(this->cor.radio.fill);
    free(this->cor.radio.espessura);
    free(this->cor.semaforo.borda);
    free(this->cor.semaforo.fill);
    free(this->cor.semaforo.espessura);
    if(this->arq_txt)
      fechaArquivo(this->arq_txt);
    free(this);
    this = NULL;

  }

}
