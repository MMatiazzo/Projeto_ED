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


int comparatorMuro(void *s1, void *s2){
  return 1;
}

int comparatorMuroAux(void *s1, void *s2){
  seg_t *this = (seg_t*) s1;
  seg_t *this2 = (seg_t*) s2;
  if(this->x1 < this2->x1){
    return -1;
  }else if(this->x1 > this2->x1){
    return 1;
  }else if(this->y1 < this2->y1){
    return -1;
  }else if(this->y1 > this2->y1){
    return 1;
  }
  return 0;
}

void apagaSegmento(segmento seg){
  free(seg);
}


void printaSvgMuroDm(void *data,void *data2,FILE *arquivoSVG,int x,int y,char cor,int tam){   
    static int lastS = 0;
    seg_t *w = data;
    seg_t *h = data2;
    char color[20];
    if(cor == 0){
        strcpy(color,"black");      
    }else if(cor == 1)
    {
        strcpy(color,"red");
    }
    fprintf(arquivoSVG,"\t<circle r='%d' cx='%d' cy='%d' stroke='%s' fill='%s' stroke-width='1.0' fill-opacity='1'/>\n",10,x,y,color,color);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'> </text>\n",x,y+15);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>x = %f</text>\n",x,y+20,w->x1);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>y = %f</text>\n",x,y+25,w->y1);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>x = %f</text>\n",x,y+30,w->x2);
    fprintf(arquivoSVG,"\t<text x='%d' y='%d' text-anchor='middle' fill-opacity='1' font-size='4px'>y = %f</text>\n",x,y+35,w->y2);

    if(w->x1 < lastS)
        //printf("\nMUTE\n");
    lastS = w->x1;
    //return NULL;
}
