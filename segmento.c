#include "segmento.h"
#include <stdlib.h>

typedef struct segmento{
  float x1,y1,
        x2,y2;
}seg_t;

segmento criaSegmento(float x1, float y1, float x2, float y2){
  seg_t *this;
  this = malloc(sizeof(seg_t));
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  return (segmento)this;
}

float segGetX1(segmento s){
  seg_t *this;
  this = (seg_t *) s;
  return this->x1;
}

float segGetY1(segmento s){
  seg_t *this;
  this = (seg_t *) s;
  return this->y1;
}

float segGetX2(segmento s){
  seg_t *this;
  this = (seg_t *) s;
  return this->x2;
}

float segGetY2(segmento s){
  seg_t *this;
  this = (seg_t *) s;
  return this->y2;
}




void apagaSegmento(segmento seg){
  free(seg);
}
