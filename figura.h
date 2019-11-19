#ifndef _FIGURA_H_
#define _FIGURA_H_

typedef void * figura;

enum tipo_figura{CIRCULO, RETANGULO};

figura criaCirculo(int id, float raio, float x, float y, char* cor1, char* cor2, char* espessura);

figura criaRetangulo(int id, float w, float h, float x, float y, char* cor1, char* cor2, char* espessura);

figura criaBoundBox(figura fig, char *cor);

int comparatorFig(figura f1, figura f2);

void apagaFigura(figura fig);

enum tipo_figura getTipoFigura(figura fig);

char *getId(figura fig);

float getXfig(figura fig);

float getYfig(figura fig);

float getR(figura fig);

float getW(figura fig);

float getH(figura fig);

char * getCor1(figura fig);

void setCor1(figura fig, char *cor1);

char * getCor2(figura fig);

char * getEspessura(figura fig);

void setX(figura fig, float x);

void setY(figura fig, float y);



#endif
