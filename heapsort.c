#include "heapsort.h"
#include "equipamento.h"
#include <stdio.h>
//
// void peneira(void **vet, int raiz, int fundo,int (*f)(void*,void*,float,float),float x, float y);
//
// void heapsort(void **vet, int n, int (*f)(void*,void*,float,float),float x, float y) {
//   int i;
//   void* tmp;
//    printf("p2:%s\n",(getSemaforoId(vet[0])));
//   // printf("teste:%d\n", comparaSemaforoPonto(getItem(l,0), getItem(l,1),5,5));
//   // printf("TAMANHO:%d\n", n);
// 	for (i = (n / 2); i >= 0; i--) {
// 		peneira(vet, i, n - 1,f,x,y);
// 	}
//
// 	for (i = n-1; i >= 1; i--) {
// 		tmp = vet[i];
// 		vet[0] = vet[i];
// 		vet[i] = tmp;
// 		peneira(vet, 0, i-1,f,x,y);
// 	}
// }
//
// void peneira(void **vet, int raiz, int fundo,int (*f)(void*,void*,float,float), float x, float y) {
// 	int pronto, filhoMax;
//   void* tmp;
//   // printf("p3:%p\n",this->node[0].item);
//
// 	pronto = 0;
//   // printf("RAIZ:%d\n", raiz);
//
// 	while ((raiz*2 <= fundo) && (!pronto)) {
// 		if (raiz*2 == fundo) {
// 			filhoMax = raiz * 2;
// 		}
// 		// else if (vet[raiz * 2] > vet[raiz * 2 + 1]) {
// 		else if ((*f)(vet[raiz*2],vet[raiz*2+1],x,y) > 0){
// 			filhoMax = raiz * 2;
// 		}
// 		else {
// 			filhoMax = raiz * 2 + 1;
// 		}
//
// 	// if (!((*f)(this->node[raiz].item,this->node[filhoMax].item,x,y))) {
// 	if (!((*f)(vet[raiz],vet[filhoMax],x,y))) {
// 		tmp = vet[raiz];
// 		vet[raiz] = vet[filhoMax];
// 		vet[filhoMax] = tmp;
// 		raiz = filhoMax;
//     }
// 	else {
//       pronto = 1;
// 	}
//   }
// }

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
          // if ((filho + 1 < n)  &&  (a[filho + 1] > a[filho]))
          if ((filho + 1 < n)  &&  ((*f)(a[filho + 1],a[filho],x,y) > 0))
              filho++;
          // if (a[filho] > t) {
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
