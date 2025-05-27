#include "../headers/pergunta.h"
#include "../headers/leitor_csv.h"
#include "../headers/funcoes_padrao.h"

//Aleatoriza uma pergunta
void aleatorizaPergunta(Pergunta *pergunta, int nivel){
     Pergunta *nova = malloc(sizeof(Pergunta));
     if(nova == NULL){
          printf("Erro ao alocar memória!\n");
          return;
     }//if

     // Seleciona as questões de acordo com o nível
     for (int i = 0; i < 4; i++){
          if(pergunta->nivel == nivel){
               for(int j = 0; j < 4; j++){
                    nova->alternativas[i].letra = pergunta->alternativas[i].letra;
                    nova->alternativas[i].texto = malloc(strlen(pergunta->alternativas[i].texto) + 1);
               }//for
               strcpy(nova->alternativas[i].texto, pergunta->alternativas[i].texto);
          }//if

     }//for
}//aleatorizaPergunta