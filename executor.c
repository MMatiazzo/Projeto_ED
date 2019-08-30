#include "executor.h"
#include "listaed.h"
#include "figura.h"
#include "quadra.h"
#include "hidrante.h"
#include "semaforo.h"
#include "radio.h"
#include "predio.h"
#include "segmento.h"
#include "texto.h"
#include "funcoes.h"
#include "svg.h"
#include "arquivo.h"
#include "free.h"
#include "heapsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct executor{
  char* dir_base;
  char* nome_entrada;
  char* nome_consulta;
  char* dir_saida;

  lista figuras;
  lista quadras;
  lista hidrantes;
  lista semaforos;
  lista radios;
  lista predios;
  lista muros;

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
  this->arq_txt = NULL;
  this->nome_entrada  = NULL;
  this->nome_consulta = NULL;
  this->dir_saida     = NULL;
  this->dir_base      = (char *) malloc(3 * sizeof(char));
  strcpy(this->dir_base, "./");
  this->figuras     = NULL;
  this->quadras     = NULL;
  this->hidrantes   = NULL;
  this->semaforos   = NULL;
  this->radios      = NULL;
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
  // printf("CAMINHO: %s\n", path);
  this->arq_svg = criaSVG(path);
  free(nome_base);
  free(dirsaida);
  free(path);

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
  this->figuras     = criaLista(nf);
  this->quadras     = criaLista(nq);
  this->hidrantes   = criaLista(nh);
  this->semaforos   = criaLista(ns);
  this->radios      = criaLista(nr);
  this->predios     = criaLista(np);
  this->muros       = criaLista(nm);
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
  lista bbox;
  svg svg_bbox;
  node lista_node;

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
        this->figuras = insere_lista(this->figuras, new_fig);
        this->maximos.total_figuras++;
      break;


      case CRIA_RETANGULO:
        if(this->maximos.total_figuras >= this->maximos.figuras) break;
        new_fig = criaRetangulo(atoi(parametros[0]), atof(parametros[1]), atof(parametros[2]), atof(parametros[3]), atof(parametros[4]), parametros[5], parametros[6], this->espessuras.retangulo);
        this->figuras = insere_lista(this->figuras, new_fig);
        this->maximos.total_figuras++;
      break;

      case TODO_TEXTO:

        aux = todoTextoFunc(total_parametros, parametros);
        new_texto = criaTexto(atof(parametros[0]), atof(parametros[1]), aux);
        escreveTexto(this->arq_svg, new_texto);
        apagaTexto(new_texto);
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
        this->quadras = insere_lista(this->quadras, new_fig);
        this->maximos.total_quadras++;
      break;


      case INSERE_HIDRANTE:
        if(this->maximos.total_hidrantes >= this->maximos.hidrantes) break;
        new_fig = criaHidrante(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.hidrante.borda, this->cor.hidrante.fill, this->cor.hidrante.espessura);
        this->hidrantes = insere_lista(this->hidrantes, new_fig);
        this->maximos.total_hidrantes++;
      break;


      case INSERE_SEMAFORO:
        if(this->maximos.total_semaforos >= this->maximos.semaforos) break;
        new_fig = criaSemaforo(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.semaforo.borda, this->cor.semaforo.fill, this->cor.semaforo.espessura);
        this->semaforos = insere_lista(this->semaforos, new_fig);
        this->maximos.total_semaforos++;
      break;


      case INSERE_RADIO:
      if(this->maximos.total_radios >= this->maximos.radios) break;
      new_fig = criaRadio(parametros[0], atof(parametros[1]), atof(parametros[2]), this->cor.radio.borda, this->cor.radio.fill, this->cor.radio.espessura);
      this->radios = insere_lista(this->radios, new_fig);
      this->maximos.total_radios++;
      break;


      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3      //      FASE 3

      case CRIA_PREDIO:
      if(this->maximos.total_predios >= this->maximos.predios) break;
      if(!(new_fig2 = percorreLista(this->quadras, PROCURA_QUADRA_ID, parametros[0]))) break;
      new_fig2 = getQuadraRect(new_fig2);
      new_fig = criaPredio(parametros[0], parametros[1], atof(parametros[2]), atof(parametros[3]), atof(parametros[4]), atof(parametros[5]) , new_fig2);
      this->predios = insere_lista(this->predios, new_fig);
      this->maximos.total_predios++;
      break;


      case CRIA_MURO:
      if(this->maximos.total_muros >= this->maximos.muros) break;
      new_fig = criaSegmento(atof(parametros[0]), atof(parametros[1]), atof(parametros[2]), atof(parametros[3]));
      this->muros = insere_lista(this->muros, new_fig);
      this->maximos.total_muros++;
      break;

      default: break;         //FIM DOS COMANDOS


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
  lista bbox;
  svg svg_bbox;
  node lista_node;
  void **vet_s, **vet_h, **vet_q;


  parametros = getComandoParametros(cmd);
  total_parametros = getNumeroParametros(cmd);

  switch(tipo){
    case SOBREPOSICAO:
    new_fig = procuraFigura(this->figuras, atoi(parametros[0]));
    new_fig2 = procuraFigura(this->figuras, atoi(parametros[1]));

    if( !new_fig || !new_fig2){
      // printf("Figura não encontrada\n");
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
      new_fig = procuraFigura(this->figuras, atoi(parametros[0]));
      char *cor;
      aux = (char *) malloc(sizeof(char) * (strlen(parametros[0]) + (strlen(parametros[1])) + strlen(parametros[2]) + 19));
      cor = (char *) malloc(sizeof(char) * 6);
      sprintf(aux, "i? %s %s %s\n", parametros[0], parametros[1], parametros[2]);
      strcpy(cor, "red");

      if(!new_fig){
        // printf("Figura não encontrada\n");
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
      new_fig = procuraFigura(this->figuras, atoi(parametros[0]));
      new_fig2 = procuraFigura(this->figuras, atoi(parametros[1]));
      texto text;
      aux = (char *) malloc(sizeof(char) * strlen(parametros[0]) + strlen(parametros[1]) + 6);
      sprintf(aux, "d? %s %s\n", parametros[0], parametros[1]);
      if(!new_fig || !new_fig2){
        // printf("Figura não encontrada\n");
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
      printaLista(bbox );
      aux2 = removeExtensao(this->nome_entrada);
      dir_saida = colocaBarraFinal(this->dir_saida);
      aux = (char *) malloc(sizeof(char) * (strlen(parametros[0]) + strlen(aux2) + strlen(dir_saida) + 6));
      sprintf(aux, "%s%s-%s.svg", dir_saida, aux2, parametros[0]);
      svg_bbox = criaSVG(aux);

      lista_node = getInicioLista(this->figuras);

      while(temProximo(lista_node)){
        new_fig = (figura) getItemProx(this->figuras, &lista_node);
        desenhaFigura(svg_bbox, new_fig);
      }

      lista_node = getInicioLista(bbox);

      while(temProximo(lista_node)){
        new_fig = (figura) getItemProx(bbox, &lista_node);
        desenhaFigura(svg_bbox, new_fig);
      }
      apagaListaGeral(bbox, FIGURA);
      free(aux);
      free(aux2);
      free(dir_saida);
      fechaSVG(svg_bbox);
    break;



    case DELETA_OBJETOS:
      aux = deleteObj(this->quadras, this->hidrantes, this->semaforos, this->radios, parametros[0]);
      if(aux){
        aux2 = (char *)malloc(sizeof(char) * strlen(aux) + 10);
        sprintf(aux2, "Removido %s", aux);
        escreveLinha(this->arq_txt, aux2);
        free(aux);
        free(aux2);
      }
    break;


    case COORDENADAS_ESPECIE:
      aux = procuraObj(this->quadras, this->hidrantes, this->semaforos, this->radios, parametros[0]);
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;


    case COR_BORDA_QUADRA:
      aux = cbqFunc(this->quadras, atof(parametros[0]), atof(parametros[1]), atof(parametros[2]), parametros[3]);
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;

    case DQ_REMOVE_QUADRA:
      aux = dqFunc(this->arq_svg, this->quadras, this->hidrantes, this->semaforos, this->radios, parametros[0], parametros[1], atof(parametros[2]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;


    case TRANSLOCA:
      aux = translocafunc(this->arq_svg, this->quadras, this->hidrantes, this->semaforos, this->radios, atof(parametros[0]), atof(parametros[1]),atof(parametros[2]),atof(parametros[3]),atof(parametros[4]),atof(parametros[5]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
    break;


    case FOCO_INCENDIO:
      // heapsort(this->semaforos, getTamanho(this->semaforos),comparaSemaforoPonto, atof(parametros[0]), atof(parametros[1]));
      vet_s = toVect(this->semaforos);
      vet_h = toVect(this->hidrantes);
      // for(int i = 0; i < getTamanho(this->semaforos); i++){
      //   printf("TESTE SEMA:%s\n", getSemaforoId(vet_s[i]));
      // }
      heapsort(vet_s,getTamanho(this->semaforos),semaforoComparator, atof(parametros[0]), atof(parametros[1]));
      heapsort(vet_h,getTamanho(this->hidrantes),hidranteComparator, atof(parametros[0]), atof(parametros[1]));
      aux = fIFunction(this->arq_svg, vet_s, vet_h, getTamanho(this->semaforos), getTamanho(this->hidrantes),atof(parametros[0]),atof(parametros[1]),atoi(parametros[2]),atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }

      free(vet_s);
      free(vet_h);
    break;

    case FOCO_SEMAFORO:
      vet_s = toVect(this->semaforos);
      aux = fSFunction(this->arq_svg, vet_s, this->quadras, getTamanho(this->semaforos), atoi(parametros[0]), parametros[1], parametros[2], atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }

      free(vet_s);

    break;


    case FOCO_HIDRANTE:
      vet_h = toVect(this->hidrantes);
      aux = fHFunction(this->arq_svg, vet_h, this->quadras, getTamanho(this->hidrantes), atoi(parametros[0]), parametros[1], parametros[2], atof(parametros[3]));
      if(aux){
        escreveLinha(this->arq_txt, aux);
        free(aux);
      }
      free(vet_h);
    break;

  }
}


void escreveTudoSVG(executor exec){
  struct executor *this;
  this = (struct executor *)exec;

  percorreLista(this->quadras   ,DESENHA_QUADRA  , this->arq_svg);
  percorreLista(this->figuras   ,DESENHA_FIGURA  , this->arq_svg);
  percorreLista(this->hidrantes ,DESENHA_HIDRANTE, this->arq_svg);
  percorreLista(this->semaforos ,DESENHA_SEMAFORO, this->arq_svg);
  percorreLista(this->radios    ,DESENHA_RADIO   , this->arq_svg);
  percorreLista(this->predios   ,DESENHA_PREDIO  , this->arq_svg);
  percorreLista(this->muros     ,DESENHA_MURO    , this->arq_svg);

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


void finalizaQRY(executor exec){
  struct executor *this;
  figura figura_atual;
  texto texto_atual;
  node lista_node;
  arquivo arquivo_txt;
  this = (struct executor *)exec;

  percorreLista(this->figuras    , DESENHA_FIGURA    , this->arq_svg);
  percorreLista(this->quadras    , DESENHA_QUADRA    , this->arq_svg);
  percorreLista(this->hidrantes  , DESENHA_HIDRANTE  , this->arq_svg);
  percorreLista(this->semaforos  , DESENHA_SEMAFORO  , this->arq_svg);
  percorreLista(this->radios     , DESENHA_RADIO     , this->arq_svg);
  percorreLista(this->predios    , DESENHA_PREDIO    , this->arq_svg);
  percorreLista(this->muros      , DESENHA_MURO      , this->arq_svg);
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

    // apagaListaGeral(this->textos, TEXTO);
    // apagaListaGeral(this->textos_qry, TEXTO);
    // apagaListaGeral(this->saida_txt, STRING);
    apagaListaGeral(this->figuras, FIGURA);
    // apagaListaGeral(this->figuras_qry, FIGURA);
    // apagaListaGeral(this->linhas_qry, STRING);
    apagaListaGeral(this->quadras, QUADRA);
    apagaListaGeral(this->hidrantes, HIDRANTE);
    apagaListaGeral(this->semaforos, SEMAFORO);
    apagaListaGeral(this->radios, RADIO);
    apagaListaGeral(this->muros, MURO);
    apagaListaGeral(this->predios, PREDIO);
    if(this->arq_txt)
      fechaArquivo(this->arq_txt);
    free(this);
    this = NULL;

  }

}
