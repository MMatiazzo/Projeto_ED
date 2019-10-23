#ifndef _FREE_H_
#define _FREE_H_
#include "listaed.h"

enum tipo_free{ FIGURA, TEXTO, STRING, QUADRA, HIDRANTE_F, SEMAFORO_F, RADIO_F, PREDIO, MURO};

void apagaListaGeral(lista list, enum tipo_free tipo);


#endif
