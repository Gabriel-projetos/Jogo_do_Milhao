#include <time.h>
#include "../headers/pergunta.h"
#include "../headers/jogo.h"

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade
 * 
 * @param perguntas Array de ponteiros para Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 */
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel) {
    
    //Variaveis
    Pergunta *filtradas[total];
    int count = 0;

    //Copia as perguntas do nível
    for (int i = 0; i < total; i++) {
        if ((int)perguntas[i].nivel == nivel) {
            filtradas[count++] = &perguntas[i];
        }//if
    }//for

    if (count == 0) return NULL;

    //Sorteia a Pergunta
    srand((unsigned)time(NULL));
    int idx = rand() % count;
    return filtradas[idx];
}//sorteiaPorNivel

/**
 * brief loop interno que mantem o jogo rodando
 * 
 * @param perguntas Array de ponteiros para Pergunta
 * @param total Número total de perguntas
 */

 void jogoAcontece(Pergunta perguntas[], int total) {
    //Variaveis
    int nivel = 1, acerto = 0;
    char resposta;

    //Loop do Jogo
    while(1){
        /*====== Perguntas do Nivel Muito Facil=====*/
        while (acerto < 2){
            perguntas = sorteiaPorNivel(perguntas, total, nivel);
            if (perguntas == NULL) printf("Nenhuma pergunta encontrada para o nivel!");

            //Mostra Pergunta
            mostraPergunta(perguntas);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == perguntas->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while

        /*====== Perguntas do Nivel Facil=====*/
        while (acerto < 4){
            perguntas = sorteiaPorNivel(perguntas, total, nivel);
            if (perguntas == NULL) printf("Nenhuma pergunta encontrada para o nivel!");

            //Mostra Pergunta
            mostraPergunta(perguntas);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == perguntas->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while

        /*====== Perguntas do Nivel Medio=====*/
        while (acerto < 8){
            perguntas = sorteiaPorNivel(perguntas, total, nivel);
            if (perguntas == NULL) printf("Nenhuma pergunta encontrada para o nivel!");

            //Mostra Pergunta
            mostraPergunta(perguntas);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == perguntas->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
                return; // Encerra o jogo se errar
            }//if e else

            nivel++;
        }//while

        /*====== Perguntas do Nivel Dificil=====*/
        while (acerto < 12){
            perguntas = sorteiaPorNivel(perguntas, total, nivel);
            if (perguntas == NULL) printf("Nenhuma pergunta encontrada para o nivel!");

            //Mostra Pergunta
            mostraPergunta(perguntas);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == perguntas->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
                return; // Encerra o jogo se errar
            }//if e else

            nivel++;
        }//while

        /*====== Perguntas do Nivel Muito Dificil=====*/
        while (acerto < 16){
            perguntas = sorteiaPorNivel(perguntas, total, nivel);
            if (perguntas == NULL) printf("Nenhuma pergunta encontrada para o nivel!");
            
            if (acerto == 15){
                perguntaDoMilhao(perguntas);
                return;
            }//if

            //Mostra Pergunta
            mostraPergunta(perguntas);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == perguntas->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while
    }//while
 }//jogoAcontece


 /**
  * @brief Função que exibe a pergunta do Milhão 
  * @param perguntas Array de ponteiros para Pergunta
  */
 void perguntaDoMilhao(Pergunta perguntas[]) {
    //Variaveis
    char resposta;

    //Mostra Pergunta
    printf("\033[1;33mParabéns! Você chegou à pergunta do milhão!\033[0m\n");
    mostraPergunta(perguntas);
    printf("Digite a letra da alternativa correta: ");
    limpaBuffer();
    scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
    resposta = toupper(resposta);
    
    if (resposta == perguntas->correta) {
        printf("\033[1;32mParabéns! Você ganhou o jogo!\033[0m\n");
    } else {
        printf("\033[1;31mErrado! A resposta correta era %c.\033[0m\n", perguntas->correta);
    }//if e else
 } //perguntaDoMilhao

 /**
  * @brief Função que libera todas as perguntas alocadas e encerra o jogoAcontece
  * @param perguntas Array de ponteiros para Pergunta
  */
 void liberaRecursos(Pergunta perguntas[]) {
    for (int i = 0; i < 5; i++) {
        free(perguntas[i].enunciado);
        for (int j = 0; j < 4; j++) {
            free(perguntas[i].alternativas[j].texto);
        }
    }
    free(perguntas);
}//liberaRecursos