#include <stdlib.h>
#include <stdio.h>
#include "executor.h"
#include "arquivo.h"
#include "comando.h"
#include "funcoes.h"


int main(int argc, char* argv[]){
  executor exec;
  arquivo arquivo_geo, arquivo_qry, arquivo_pm = NULL, arquivo_ec = NULL;
  comando cmd;
  char *caminho, *linha, *caminho_pm, *caminho_ec;
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

    //caso tenha -pm
    caminho_pm = getCaminhoBasePessoa(exec);
    if(caminho_pm)
      arquivo_pm = abreArquivo(caminho_pm, LEITURA);
    if(!arquivo_pm){
      printf("SEM ARQUIVO PARA PESSOAS\n");
    }else{
      while(linha = lerLinha(arquivo_pm)){
        cmd = criaComandoPm(linha);
        executarComandoPm(exec, cmd);
        apagaComandoGeo(cmd);
        free(linha);
      }
    }


    //caso tenha -ec
    caminho_ec = getCaminhoBaseEstab(exec);
    if(caminho_ec)
      arquivo_ec = abreArquivo(caminho_ec,LEITURA);
    if(!arquivo_ec){
      printf("SEM ARQUIVO PARA ESTABELECIMENTO\n");
    }else{
      while(linha = lerLinha(arquivo_ec)){
        cmd = criaComandoEc(linha);
        executarComandoEc(exec, cmd);
        apagaComandoGeo(cmd);
        free(linha);
      }
    }

    escreveTudoSVG(exec);
    fechaArquivo(arquivo_geo);

    if(temQRY(exec)){
      caminho = getCaminhoQRY(exec);
      arquivo_qry = abreArquivo(caminho, LEITURA);
      free(caminho);
      if(!arquivo_qry){
        printf("ARQUIVO NAO ABERTO\n");

      }else{
        while(linha = lerLinha(arquivo_qry)){
          cmd = criaComando(linha);
          executarComandoQry(exec, cmd);
          apagaComandoGeo(cmd);
          free(linha);
        }
      }
      finalizaQRY(exec);
      fechaArquivo(arquivo_qry);
    }

    if(temInterativo(exec)){
      executarComandoI(exec);
    }
  }
  apagaExecutor(exec);
  return 0;
}
