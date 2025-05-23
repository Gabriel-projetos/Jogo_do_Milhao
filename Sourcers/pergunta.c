#include <stdio.h>
#include <string.h>
#include "../Headers/pergunta.h"

void recebePergunta(Pergunta **perguntas, int total_perguntas){     
     *perguntas = realloc(*perguntas, (total_perguntas + 1) * sizeof(Pergunta));
     
     Pergunta *p = &(*perguntas)[total_perguntas];
     
     //Recebe o enunciado
     printf("Digite o enunciado da questão:\n");
     p->enunciado = leTextoDinamico();
     converteMaiscula(p->enunciado);

     //Recebe alternativas
     char letras[] = {'a', 'b', 'c', 'd'};
     for(int i = 0; i < 4; i++){
          p->alternativas[i].letra = letras[i];
          printf("Digite a alternativa %c)\n", p->alternativas[i].letra);
          p->alternativas[i].texto = leTextoDinamico();
          converteMaiscula(p->alternativas[i].texto);
     }//for

     //Recebe correta
     printf("Digite qual a alternativa correta:\n");
     *p->correta = leTextoDinamico();
     
     //Recebe dificuldade
     printf("Escolha a dificuldade da questão:\n");
     printf("1 - Muito Facil\n2 - Facil\n3 - Médio\n4 - Dificil\n5 - Muito Dificil\n");
     scanf("%d", (int*) &p->dificuldade);
     limpaBuffer();
     
     //limpa a memória
     for(int i = 0; i < total_perguntas; i++){
          free(p[i].enunciado);
          for(int j = 0; j < 4; j++){
               free(p[i].alternativas[j].texto);
          }//for
     }//for
     free(perguntas);
}//recebePergunta

void mostraPergunta(Pergunta perguntas){
     printf("%s\n",perguntas.enunciado);                                                                  // Mostra o Enunciado
     
     for(int i = 0; i < 4; i++){                                                                          // Mostra as alternativas
          printf("%c) %s\n", perguntas.alternativas[i].letra, perguntas.alternativas[i].texto);
     }//for
}//mostraPergunta

void listaPerguntas(Pergunta *perguntas, int total_perguntas){
     for(int nivel = 1; nivel <= 5; nivel++){
          for(int i = 0; i < total_perguntas; i++){
               if(perguntas[i].dificuldade == nivel){
                    printf("\n");
                    mostraPergunta(perguntas[i]);
               }//if
          }//for
     }//for
}//listaPerguntas

void pesquisaPergunta(Pergunta *perguntas){
     char *pesquisa;
     pesquisa = leTextoDinamico();
     converteMaiscula(pesquisa);
     for(int i = 0; i < 15; i++){
          if(strstr(perguntas[i].enunciado, pesquisa) == 0){
               mostraPergunta(perguntas[i]);
          }else{
               printf("Pergunta não encontrada!\n");
          }//ifelse
     }//for
     free(pesquisa);
}//pesquisaPergunta

void alterarPergunta(Pergunta *perguntas){
     int pergunta_troca;
     printf("Qual pergunta deseja trocar?");
     scanf("%d", &pergunta_troca);
     recebePergunta(&perguntas, pergunta_troca);
     printf("Pergunta %d trocada com sucesso!\n", pergunta_troca);
}//alterarPergunta

void excluirPergunta(Pergunta *perguntas, int total_perguntas){
     int indice;
     printf("Qual pergunta deseja excluir? ");
     scanf("%d", &indice);
     if (indice >= 0 && indice < total_perguntas) {
          free(perguntas[indice]); // libera a memória da pergunta
     for (int i = indice; i < total_perguntas - 1; i++) {
          perguntas[i] = perguntas[i + 1];
     }
     total_perguntas--;
     // Reduz o tamanho do vetor
     perguntas = realloc(perguntas, total_perguntas * sizeof(Pergunta*));
     }
}//excluirPergunta