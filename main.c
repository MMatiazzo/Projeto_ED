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
  comando cmd;
  char *caminho, *linha;
  int nx[7];

  exec = criaExecutor();
  setParametros(exec, argc, argv);
  caminho = getCaminhoBase(exec);
  arquivo_geo = abreArquivo(caminho, LEITURA);
  free(caminho);
  if(!arquivo_geo){
    printf("ARQUIVO NAO ABERTO\n");
  }else{                                  //resto do programa

    pesquisaNx(arquivo_geo, nx);
    setNx(exec, nx[0], nx[1], nx[2], nx[3], nx[4], nx[5], nx[6]);

    while(linha = lerLinha(arquivo_geo)){
      cmd = criaComando(linha);
      executarComando(exec, cmd);
      apagaComandoGeo(cmd);
      free(linha);
    }
    escreveTudoSVG(exec);


    if(temQRY(exec)){
      caminho = getCaminhoQRY(exec);
      arquivo_qry = abreArquivo(caminho, LEITURA);
      free(caminho);
      if(!arquivo_qry){
        printf("ARQUIVO NAO ABERTO\n");

      }else{
        while(linha = lerLinha(arquivo_qry)){
          cmd = criaComando(linha);
          executarComando(exec, cmd);
          apagaComandoGeo(cmd);
          free(linha);
        }
      }
      finalizaQRY(exec);
      fechaArquivo(arquivo_qry);
    }
  }
  fechaArquivo(arquivo_geo);
  apagaExecutor(exec);
  return 0;
}
