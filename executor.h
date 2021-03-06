#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_
#include "comando.h"

typedef void * executor;

enum tipo_parametro{NOME_ENTRADA, NOME_CONSULTA, DIR_BASE, DIR_SAIDA};

executor criaExecutor();

int executarComando(executor exec, comando cmd);

void executarComandoPm(executor exec, comando cmd);

void executarComandoEc(executor exec, comando cmd);

int executarComandoQry(executor exec, comando cmd);

void executarComandoI(executor exec);

int temQRY(executor exec);

int temInterativo(executor exec);

executor setParametros(executor param, int argc, char* argv[]);

executor setNx(executor exec, int nf, int nq, int nh, int ns, int nr, int np, int nm);

char* getParametros(executor param, enum tipo_parametro tipo);

char* getCaminhoBasePessoa(executor executor);

char* getCaminhoBaseEstab(executor executor);

char* getCaminhoBase(executor param);

char* getCaminhoQRY(executor executor);

void escreveTudoSVG(executor exec);

void finalizaQRY(executor exec);

void apagaExecutor(executor param);








#endif
