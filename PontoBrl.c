#include "PontoBrl.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

typedef struct  {
    float x,y;
}Point;

void* NewPoint(float x, float y){
    Point *this = malloc(sizeof(Point));
    this->x = x; 
    this->y = y;
    return this;
}

float getXPoint(void* p){
    Point *this = p;
    return this->x;
}

float getYPoint(void* p){
    Point *this = p;
    return this->y;
}
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define INSIDE 1
#define OUTSIDE 0
#define END 9999999

int InsidePolygon(void **polygon,int N,void* px)
{
    int counter = 0;
    int i;
    double xinters;
    Point *p1,*p2;
    Point *p = (Point*)px;

    p1 = (Point*)polygon[0];
    for (i=1;i<=N;i++) {
        p2 = (Point*)polygon[i % N];
        if (p->y > MIN(p1->y,p2->y)) {
            if (p->y <= MAX(p1->y,p2->y)) {
                if (p->x <= MAX(p1->x,p2->x)) {
                    if (p1->y != p2->y) {
                        xinters = (p->y-p1->y)*(p2->x-p1->x)/(p2->y-p1->y)+p1->x;
                        if (p1->x == p2->x || p->x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }
    if (counter % 2 == 0)
        return(OUTSIDE);
    else
        return(INSIDE);
}