#ifndef _SVG_H_
#define _SVG_H_
#include "figura.h"
#include "texto.h"
#include "quadra.h"
#include "equipamento.h"
#include "segmento.h"
#include "predio.h"


typedef void * svg;


svg criaSVG(char * caminho);

void fechaSVG(svg arq_svg);

void desenhaFigura(svg arq_svg, figura fig);

void escreveTexto(svg arq_svg, texto text);

void escreveLinhaSVG(svg arq_svg, char* texto);

void desenhaQuadra(svg arq_svg, quadra q);

void desenhaEquipamento(svg arq_svg, equipamento h);

void desenhaLineSVG(svg arq_svg, segmento s);

void desenhaPredio(svg arq_svg, predio p);

void desenhaEstab(svg arq_svg, void* estabelecimento);

void *getSvgArq(svg arquivo_svg);

#endif
