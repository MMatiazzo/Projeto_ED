#ifndef _EQUIPAMENTO_H_
#define _EQUIPAMENTO_H_
#include "figura.h"
#include "texto.h"
#include <stdio.h>

typedef void *equipamento;

enum tipo_equipamento{RADIO, HIDRANTE, SEMAFORO};


equipamento criaHidrante(char *id, double x, double y, char *cor1, char *cor2, char *espessura);

equipamento criaSemaforo(char *id, double x, double y, char *cor1, char *cor2, char *espessura);

equipamento criaRadio(char *id, double x, double y, char *cor1, char *cor2, char *espessura);

figura getEquipamentoCirculo(equipamento e);

texto getEquipamentoTexto(equipamento e);

char* getEquipamentoId(equipamento e);

enum tipo_equipamento getEquipamentoTipo(equipamento e);

int equipamentoComparator(equipamento e1, equipamento e2);

char *equipamentoToString(equipamento e);

void apagaEquipamento(equipamento e);

void printaSvgHidDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam);

#endif