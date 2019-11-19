#include "svg.h"
#include "funcoes.h"
#include "quadra.h"
#include "svg.h"
#include "hash.h"
#include "Tree.h"
#include "predio.h"
//#include "muro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "lista.h"

typedef struct ponto{
    double x;
    double y;
}Ponto;

typedef struct segmento{
    Ponto p1, p2;
}Segmento;

typedef struct predio{
    Segmento seg1, seg2, seg3, seg4;
}PredioStruct;

typedef struct boundaries{
    double maxX, maxY;
}Boundaries;

typedef struct{
    double maxAltura, maxLargura;
}brl_t;

int intersec(Ponto k, Ponto l, Ponto m, Ponto n){
    double det, s, t;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // não há intersecção

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    if((s > 0 && s < 1) && (t > 0 && t < 1))
        return 1;
    else
        return 0;

}

Ponto calculaPontoIntersec(Ponto k, Ponto l, Ponto m, Ponto n){

    Ponto Pi;
    double det, s;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;

    Pi.x = k.x + (l.x-k.x)*s;
    Pi.y = k.y + (l.y-k.y)*s;

    return Pi;

}

Ponto getPontoIntersecFinal(Segmento *listaSegmentos, Ponto bomba, Ponto pontoMax, int tamanhoLista){

    Ponto pontofinal;
    pontofinal.x = -1;
    pontofinal.y = -1;

    double dist, dist_aux;
    dist = dist_aux = distancia(bomba.x, bomba.y, pontoMax.x, pontoMax.y);

    for(int i=0; i<tamanhoLista; i++){
        if(intersec(listaSegmentos[i].p1, listaSegmentos[i].p2, bomba, pontoMax)){
            Ponto intersec = calculaPontoIntersec(listaSegmentos[i].p1, listaSegmentos[i].p2, bomba, pontoMax);
            dist_aux = distancia(bomba.x, bomba.y,intersec.x, intersec.y);
            if(dist_aux < dist){
                pontofinal = intersec;
                dist = dist_aux;
            }

        }

    }

    return pontofinal;

}

double getCityLength(void **quadras, void **muros, int tam_quad, int tam_muro){
    int i; 
    double maior = 0, aux;
    figura rect;
    for(i = 0; i < tam_quad; i++){
        aux = getXfig(rect = getQuadraRect(quadras[i]));
        if(aux + getW(rect) > maior){
            maior = aux + getW(rect);
        } 
    }

    for(i = 0; i < tam_muro; i++){
        aux = segGetX1(muros[i]);
        if(aux > maior)
            maior = aux;
        aux = segGetX2(muros[i]);
        if(aux > maior)
            maior = aux;
    }

    return maior;

}


double getCityHeight(void **quadras, void **muros, int tam_quad, int tam_muro){
    int i; 
    double maior = 0, aux;
    figura rect;
    for(i = 0; i < tam_quad; i++){
        aux = getYfig(rect = getQuadraRect(quadras[i]));
        if(aux + getH(rect) > maior){
            maior = aux + getH(rect);
        } 
    }

    for(i = 0; i < tam_muro; i++){
        aux = segGetY1(muros[i]);
        if(aux > maior)
            maior = aux;
        aux = segGetY2(muros[i]);
        if(aux > maior)
            maior = aux;
    }
    return maior;

}

Boundaries setBoundaries(int tam_muros,Segmento *listaSegmentos, Ponto bomba, double xMax, double yMax){

    Boundaries bound;
    int tamanhoMaximoSeg = tam_muros;

    if(bomba.x >= xMax){
        xMax += (bomba.x - xMax) + 1;
    }
    if(bomba.y >= yMax){
        yMax += (bomba.y - yMax) + 1;
    }

    if(bomba.x == 0){
        listaSegmentos[tamanhoMaximoSeg-1].p1.x = -0.1; listaSegmentos[tamanhoMaximoSeg-1].p1.y = -0.1; 
        listaSegmentos[tamanhoMaximoSeg-1].p2.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-1].p2.y = -0.1;

        listaSegmentos[tamanhoMaximoSeg-3].p1.x = -0.1; listaSegmentos[tamanhoMaximoSeg-3].p1.y = yMax - 0.1; 
        listaSegmentos[tamanhoMaximoSeg-3].p2.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-3].p2.y = yMax - 0.1;

    }else{
        listaSegmentos[tamanhoMaximoSeg-1].p1.x = 0.1; listaSegmentos[tamanhoMaximoSeg-1].p1.y = 0.1; 
        listaSegmentos[tamanhoMaximoSeg-1].p2.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-1].p2.y = 0.1;

        listaSegmentos[tamanhoMaximoSeg-3].p1.x = 0.1; listaSegmentos[tamanhoMaximoSeg-3].p1.y = yMax - 0.1; 
        listaSegmentos[tamanhoMaximoSeg-3].p2.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-3].p2.y = yMax - 0.1;

    }

    if(bomba.y == 0){

        listaSegmentos[tamanhoMaximoSeg-4].p1.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-4].p1.y = yMax - 0.1; 
        listaSegmentos[tamanhoMaximoSeg-4].p2.x = xMax -0.1; listaSegmentos[tamanhoMaximoSeg-4].p2.y = 0.2;

        listaSegmentos[tamanhoMaximoSeg-2].p1.x = -0.1; listaSegmentos[tamanhoMaximoSeg-2].p1.y = -0.1; 
        listaSegmentos[tamanhoMaximoSeg-2].p2.x = -0.1; listaSegmentos[tamanhoMaximoSeg-2].p2.y = yMax - 0.1;


    }else{

        listaSegmentos[tamanhoMaximoSeg-4].p1.x = xMax - 0.1; listaSegmentos[tamanhoMaximoSeg-4].p1.y = yMax - 0.1; 
        listaSegmentos[tamanhoMaximoSeg-4].p2.x = xMax -0.1; listaSegmentos[tamanhoMaximoSeg-4].p2.y = 0.2;

        listaSegmentos[tamanhoMaximoSeg-2].p1.x = 0.1; listaSegmentos[tamanhoMaximoSeg-2].p1.y = 0.1; 
        listaSegmentos[tamanhoMaximoSeg-2].p2.x = 0.1; listaSegmentos[tamanhoMaximoSeg-2].p2.y = yMax - 0.1;

    }

    bound.maxX = xMax;
    bound.maxY = yMax;
    
    return bound;

}

PredioStruct getSegmentoPredio(void *hashCepQuadras, char *cepPredio, char *face, double margem, double profundidade, double frente, int num){

    PredioStruct segPredio; segPredio.seg1.p1.x =0;
    double xQuadra, yQuadra, wQuadra, hQuadra, xFinal, yFinal;

    void *quadra = getHashTable(hashCepQuadras, cepPredio);

    if(quadra == NULL){
        printf("Deu ruim\n");
        exit(-1);
    }

    xQuadra = getXfig(getQuadraRect(quadra));
    yQuadra = getYfig(getQuadraRect(quadra));
    wQuadra = getW(getQuadraRect(quadra));
    hQuadra = getH(getQuadraRect(quadra));

    if(strcmp(face, "N") == 0){

        xFinal = (xQuadra + num) - (frente/2);
        yFinal = (yQuadra + hQuadra) - margem - profundidade;
        
    }else if(strcmp(face, "S") == 0){

        xFinal = (xQuadra + num) - (frente/2);
        yFinal = yQuadra + margem;

    }else if(strcmp(face, "L") == 0){

        xFinal = xQuadra + margem;
        yFinal = (yQuadra + num) - (frente/2);

    }else if(strcmp(face, "O") == 0){

        xFinal = (xQuadra + wQuadra) - margem - profundidade;
        yFinal = (yQuadra + num) - (frente/2);

    }

    if((strcmp(face, "N") == 0) || (strcmp(face, "S") == 0)){

        segPredio.seg1.p1.x = xFinal; segPredio.seg1.p1.y = yFinal;
        segPredio.seg1.p2.x = xFinal + frente; segPredio.seg1.p2.y = yFinal;

        segPredio.seg2.p1.x = xFinal; segPredio.seg2.p1.y = yFinal;
        segPredio.seg2.p2.x = xFinal; segPredio.seg2.p2.y = yFinal + profundidade;

        segPredio.seg3.p1.x = xFinal; segPredio.seg3.p1.y = yFinal + profundidade;
        segPredio.seg3.p2.x = xFinal + frente; segPredio.seg3.p2.y = yFinal + profundidade;

        segPredio.seg4.p1.x = xFinal + frente; segPredio.seg4.p1.y = yFinal + profundidade;
        segPredio.seg4.p2.x = xFinal + frente; segPredio.seg4.p2.y = yFinal;
        
    }else if((strcmp(face, "L") == 0) || (strcmp(face, "O") == 0)){

        segPredio.seg1.p1.x = xFinal; segPredio.seg1.p1.y = yFinal;
        segPredio.seg1.p2.x = xFinal + profundidade; segPredio.seg1.p2.y = yFinal;

        segPredio.seg2.p1.x = xFinal; segPredio.seg2.p1.y = yFinal;
        segPredio.seg2.p2.x = xFinal; segPredio.seg2.p2.y = yFinal + frente;

        segPredio.seg3.p1.x = xFinal; segPredio.seg3.p1.y = yFinal + frente;
        segPredio.seg3.p2.x = xFinal + profundidade; segPredio.seg3.p2.y = yFinal + frente;

        segPredio.seg4.p1.x = xFinal + profundidade; segPredio.seg4.p1.y = yFinal + frente;
        segPredio.seg4.p2.x = xFinal + profundidade; segPredio.seg4.p2.y = yFinal;

    }

    return segPredio;

}

Segmento* getListaSegmento(void *hashQuadras,void **vet_predios, void **vet_muros, int tam_predios, int tam_muros ,int tamanhoMaximoMuros, int tamanhoMaximoPredios, int tamanhoListaSegmentos){

    Segmento *listaSegmentos = malloc(sizeof(Segmento) * tamanhoListaSegmentos);

    // void *listaMuros = rbParaLista(arvoreMuros);
    // void *listaPredios = rbParaLista(arvorePredios);
    for(int i=0; i < tam_muros; i++){

            if(i > tamanhoMaximoMuros){
                break;
            }
            if(vet_muros[i] != NULL){
                listaSegmentos[i].p1.x = segGetX1(vet_muros[i]);
                listaSegmentos[i].p1.y = segGetY1(vet_muros[i]);
                listaSegmentos[i].p2.x = segGetX2(vet_muros[i]);
                listaSegmentos[i].p2.y = segGetY2(vet_muros[i]);
            }
    }


    int j = tamanhoMaximoMuros;

    for(int i=0; i < tam_predios; i++){

        if(i > tamanhoMaximoPredios){
            break;
        }
        if(vet_predios[i] != NULL){

            PredioStruct segmentosPredio = getSegmentoPredio(hashQuadras,getPredioCep(vet_predios[i]), getPredioFace(vet_predios[i]), getPredioMargem(vet_predios[i]), 
                                        getPredioProfundidade(vet_predios[i]), getPredioFrente(vet_predios[i]), getPredioNum(vet_predios[i]));

            listaSegmentos[j] = segmentosPredio.seg1;
            listaSegmentos[j+1] = segmentosPredio.seg2;
            listaSegmentos[j+2] = segmentosPredio.seg3;
            listaSegmentos[j+3] = segmentosPredio.seg4;

            j += 4;
        }

    }

    return listaSegmentos;

}

void brlProcedure(double xBomba, double yBomba, void* arquivo, arvore quadras, arvore muros,arvore predios, void* quadras_hash){

    void **vet_quadras = arvoreToArray(quadras);
    void **vet_muros = arvoreToArray(muros);
    void **vet_predios = arvoreToArray(predios);
    Ponto bomba;
    bomba.x = xBomba; bomba.y = yBomba;
    
    void *circ = criaCirculo(-1,5,xBomba,yBomba,"black","yellow","1");
    desenhaFigura(arquivo,circ);

    double cityHeight = getCityHeight(vet_quadras, vet_muros, getArvoreTamanho(quadras), getArvoreTamanho(muros));
    double cityLength = getCityLength(vet_quadras, vet_muros, getArvoreTamanho(quadras), getArvoreTamanho(muros));

    printf("\n\n%lf %lf\n\n", cityHeight, cityLength);
    int tamanhoMaximoMuros = getArvoreTamanho(muros);
    int tamanhoMaximoPredios = getArvoreTamanho(quadras);
    int tamanhoMaximoSeg = tamanhoMaximoMuros + (tamanhoMaximoPredios*4) + 4;

    printf("%d %d %d", tamanhoMaximoMuros, tamanhoMaximoPredios, tamanhoMaximoSeg);

    Segmento *listaSegmentos = getListaSegmento(quadras_hash,vet_predios,vet_muros,getArvoreTamanho(predios)
    ,getArvoreTamanho(muros),tamanhoMaximoMuros, tamanhoMaximoPredios, tamanhoMaximoSeg);
    

    Boundaries bound = setBoundaries(getArvoreTamanho(muros),listaSegmentos, bomba, cityLength, cityHeight);

    
    Ponto pontomax; 

    double precisao = 0.1;
   //Esquerda
    pontomax.x = -1; pontomax.y = -1;
    
    escreveLinha(getSvgArq(arquivo),"\t<path opacity=\"0.6\" fill=\"orange\" d=\"M %lf %lf",bomba.x,bomba.y);
    while(pontomax.y <= bound.maxY){

        Ponto pontofinal = getPontoIntersecFinal(listaSegmentos, bomba, pontomax, tamanhoMaximoSeg);
        escreveLinha(getSvgArq(arquivo),"%lf %lf ",pontofinal.x,pontofinal.y);

        pontomax.y += precisao;

    }
    escreveLinha(getSvgArq(arquivo),"Z\" />\n");

   //Inferior
    escreveLinha(getSvgArq(arquivo),"\t<path opacity=\"0.6\" fill=\"orange\" d=\"M %lf %lf",bomba.x,bomba.y);
    pontomax.x = -1; pontomax.y = bound.maxY;
    while(pontomax.x <= bound.maxX){

        Ponto pontofinal = getPontoIntersecFinal(listaSegmentos, bomba, pontomax, tamanhoMaximoSeg);

        escreveLinha(getSvgArq(arquivo),"%lf %lf ",pontofinal.x,pontofinal.y);

        pontomax.x += precisao;

    }
    escreveLinha(getSvgArq(arquivo),"Z\" />\n");

    //Superior
    escreveLinha(getSvgArq(arquivo),"\t<path opacity=\"0.6\" fill=\"orange\" d=\"M %lf %lf",bomba.x,bomba.y);
    pontomax.x = -1; pontomax.y = -1;
    while(pontomax.x <= bound.maxX){

        Ponto pontofinal = getPontoIntersecFinal(listaSegmentos, bomba, pontomax, tamanhoMaximoSeg);

        escreveLinha(getSvgArq(arquivo),"%lf %lf ",pontofinal.x,pontofinal.y);

        pontomax.x += precisao;

    }
    escreveLinha(getSvgArq(arquivo),"Z\" />\n");

    //Direita
    escreveLinha(getSvgArq(arquivo),"\t<path opacity=\"0.6\" fill=\"orange\" d=\"M %lf %lf",bomba.x,bomba.y);
    pontomax.x = bound.maxX; pontomax.y = -1;

    while(pontomax.y <= bound.maxY){

        Ponto pontofinal = getPontoIntersecFinal(listaSegmentos, bomba, pontomax, tamanhoMaximoSeg);

        escreveLinha(getSvgArq(arquivo),"%lf %lf ",pontofinal.x,pontofinal.y);

        pontomax.y += precisao;

    }
    escreveLinha(getSvgArq(arquivo),"Z\" />\n");

    free(listaSegmentos);

}   
