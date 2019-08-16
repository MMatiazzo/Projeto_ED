#ifndef __SEG_H_
#define __SEG_H_

typedef void * segmento;


segmento criaSegmento(float x1, float y1, float x2, float y2);

float segGetX1(segmento s);

float segGetY1(segmento s);

float segGetX2(segmento s);

float segGetY2(segmento s);

void apagaSegmento(segmento seg);

#endif
