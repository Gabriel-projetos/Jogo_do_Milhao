#include <windows.h>
#include <locale.h>
#include "Headers/pergunta.h"

int main(){
     //Arruma os caracteres
     setlocale(LC_ALL, "pt_BR.UTF-8");
     SetConsoleOutputCP(CP_UTF8);

     //Variaveis
     Pergunta **perguntas;
     int op, total_perguntas;

     //Da a opção de inserir perguntas
     printf("Deseja inserir uma nova pergunta?\n");
     printf("1 - Sim\n2 - Não\n");
     switch(op){
          case 1:
               total_perguntas++;
               for(int i = 0; i < total_perguntas; i++){
                    recebePergunta(perguntas, total_perguntas);
                    break;
               }//for     
               break;
          case 2:
               printf("Continuando programa!\n");
               break;
          default:
               printf("Opção inválida!\n");
               break;  
     }//Switch

     //Opção de listar perguntas
     printf("Deseja listar as perguntas?\n");
     printf("1 - Sim\n2 - Não\n");
     sacanf("%d", &op);
     switch(op){
          case 1:
               listaPerguntas(perguntas, total_perguntas);
               break;
          case 2:
               printf("Continuando programa!\n");
               break;
          default:
               printf("Opção inválida!\n");
               break;
     }//switch

     //Opção de pesquisar perguntas
     printf("Deseja pesquisar uma pergunta?\n");
     printf("1 - Sim\n2 - Não\n");
     scanf("%d", &op);
     switch(op){
          case 1:
               pesquisaPergunta(perguntas);
               break;
          case 2:
               printf("Continuando programa!\n");
               break;
          default:
               printf("Opção inválida!\n");
               break;
     }//switch

     //Opção de trocar perguntas 
     printf("Deseja trocar uma pergunta?\n");
     printf("1 - Sim\n2 - Não\n");
     scanf("%d", &op);
     switch(op){
          case 1:
               alterarPergunta(perguntas);
               break;
          case 2:
               printf("Continuando programa!\n");
               break;
          default:
               printf("Opção inválida!\n");
               break;
     }//switch

     //Opção de excluir perguntas
     printf("Deseja excluir uma pergunta?\n");
     printf("1 - Sim\n2 - Não\n");
     scanf("%d", &op);
     switch(op){
          case 1:
               excluirPergunta(perguntas, total_perguntas);
               break;
          case 2:
               printf("Continuando programa!\n");
               break;
          default:
               printf("Opção inválida!\n");
               break;
     }//switch

     //limpa a memória
     for(int i = 0; i < total_perguntas; i++){
          free(perguntas[i]->enunciado);
          for(int j = 0; j < 4; j++){
               free(perguntas[i]->alternativas[j].texto);
          }//for
     }//for
     free(perguntas);
     return 0;
}//main