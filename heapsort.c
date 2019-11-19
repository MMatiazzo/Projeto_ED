#include "heapsort.h"
#include "equipamento.h"
#include <stdio.h>

void heapsort(void **a, int n, int (*f)(void*,void*,float,float),float x, float y) {
   int i = n / 2, pai, filho;
   void *t;
   while(1) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n <= 0) return;
          t = a[n];
          a[n] = a[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < n) {
          if ((filho + 1 < n)  &&  ((*f)(a[filho + 1],a[filho],x,y) > 0))
              filho++;
          if ((*f)(a[filho],t,x,y) > 0) {

             a[pai] = a[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      a[pai] = t;
   }
}
