#include "free.h"
#include "figura.h"
#include "texto.h"
#include "quadra.h"
#include "equipamento.h"
#include "predio.h"
#include <stdlib.h>

void apagaListaGeral(lista list, enum tipo_free tipo){
  node lista_node, node_free;
  void * item_atual;
  lista_node = getInicioLista(list);

    while(temProximo(lista_node)){
      node_free = lista_node;
      item_atual = getItemProx(list, &lista_node);

      switch (tipo) {
        case FIGURA : apagaFigura(item_atual); break;

        case TEXTO : apagaTexto(item_atual); break;

        case QUADRA : apagaQuadra(item_atual); break;

        case HIDRANTE_F : apagaEquipamento(item_atual); break;

        case SEMAFORO_F : apagaEquipamento(item_atual); break;

        case RADIO_F : apagaEquipamento(item_atual); break;

        case PREDIO: apagaPredio(item_atual); break;

        case MURO: free(item_atual); break;

        default : free(item_atual);
      }
    }
  apagaLista(list);
}
