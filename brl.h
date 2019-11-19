#ifndef __BRL_H
#define __BRL_H

/* As outras funções do brl.c são de uso exclusivo do arquivo e por isso
não estão declaradas neste .h */

/* Função responsável pela chamada de funções do cálculo da região de visibilidade
e impressão da região no arquivo SVG */
void brlProcedure(double xBomba, double yBomba, void* arquivo, void* quadras, void* muros,void* predios, void* quadras_hash);
#endif 