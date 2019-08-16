#include <stdlib.h>
#include <stdio.h>
#include "executor.h"
#include "arquivo.h"
#include "comando.h"
#include "free.h"
#include "funcoes.h"


int main(int argc, char* argv[]){
  executor exec;
  arquivo arquivo_geo, arquivo_qry;
  char *caminho;
  lista linhas;
  int nx[5], total_linhas;

  exec = criaExecutor();
  setParametros(exec, argc, argv);
  caminho = getCaminhoBase(exec);
  arquivo_geo = abreArquivo(caminho, LEITURA);
  free(caminho);
  if(!arquivo_geo){
    printf("ARQUIVO NAO ABERTO\n");
  }else{                                  //resto do programa

    total_linhas = pesquisaNx(arquivo_geo, nx);
    setNx(exec, nx[0], nx[1], nx[2], nx[3], nx[4]);
    linhas = recebeLinhas(arquivo_geo, total_linhas);
    percorreLista(linhas, EXECUTA_COMANDO, exec);
    apagaListaGeral(linhas, STRING);
    escreveTudoSVG(exec);


    if(temQRY(exec)){
      caminho = getCaminhoQRY(exec);
      arquivo_qry = abreArquivo(caminho, LEITURA);
      free(caminho);
      if(!arquivo_qry){
        printf("ARQUIVO NAO ABERTO\n");

      }else{
        total_linhas = contaLinhas(arquivo_qry);
        linhas = recebeLinhas(arquivo_qry, total_linhas);
        percorreLista(linhas, EXECUTA_COMANDO_QRY, exec);
        apagaListaGeral(linhas, STRING);
      }
      finalizaQRY(exec);
      fechaArquivo(arquivo_qry);
    }
  }
  fechaArquivo(arquivo_geo);
  apagaExecutor(exec);
  return 0;
}
