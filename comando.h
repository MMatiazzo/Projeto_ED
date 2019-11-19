#ifndef _COMANDO_GEO_H_
#define _COMANDO_GEO_H_

enum tipo_comando {
  NONE,
  ALTERA_MAXIMOS,
  CRIA_CIRCULO,
  CRIA_RETANGULO,
  TODO_TEXTO,
  SOBREPOSICAO,
  PONTO_INTERNO,
  DISTANCIA_CENTRO,
  CRIA_BOUNDING_BOX,
  INSERE_QUADRA,
  INSERE_HIDRANTE,
  INSERE_SEMAFORO,
  INSERE_RADIO,
  MUDA_PADRAO_QUADRA,
  MUDA_PADRAO_HIDRANTE,
  MUDA_PADRAO_RADIO,
  MUDA_PADRAO_SEMAFORO,
  ESPESSURA_FIGURAS,
  DELETA_OBJETOS,
  COORDENADAS_ESPECIE,
  COR_BORDA_QUADRA,
  DQ_REMOVE_QUADRA,
  TRANSLOCA,
  CRIA_PREDIO,
  CRIA_MURO,
  FOCO_INCENDIO,
  FOCO_HIDRANTE,
  FOCO_SEMAFORO,
  CADASTRA_PESSOA,
  ENDERECO_PESSOA,
  DEFINE_ESTAB,
  INSERE_ESTAB,
  IMPRIME_MORADOR,
  IMPRIME_ESTAB_MOR,
  MUDAR_ENDERECO,
  LISTAR_MORADORES,
  PRINTA_ARVORE,
  NAVEGA_IT,
  QRY_IT,
  SAIR_IT,
  PONTO_LUZ,
  POLIGONO_BORDA,
  FINALIZA,
};

typedef void* comando;

comando criaComando(char * entrada);

comando criaComandoPm(char * entrada);

comando criaComandoEc(char * entrada);

comando criaComandoIt(char * entrada);

enum tipo_comando getTipoComando(comando cmd);

int getNumeroParametros(comando cmd);

char **getComandoParametros(comando cmd);

void apagaComandoGeo(comando cmd);





#endif
