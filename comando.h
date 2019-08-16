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
  FINALIZA,
};

typedef void* comando;

comando criaComando(char * entrada);

enum tipo_comando getTipoComando(comando cmd);

int getNumeroParametros(comando cmd);

char **getComandoParametros(comando cmd);

void apagaComandoGeo(comando cmd);





#endif
