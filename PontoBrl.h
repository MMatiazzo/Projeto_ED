#ifndef _Ponto_H
#define _Ponto_H
#include<stdio.h>

//Tad para receber todos os pontos das quadras e muros


//Cria novo ponto
void* NewPoint(float x, float y);
float getXPoint(void* p);
float getYPoint(void* p);
void *criaVetPonto(int tam);
int InsidePolygon(void **polygon,int N,void* p);
#endif