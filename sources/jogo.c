#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include "../Headers/pergunta.h"
#include "../Headers/jogo.h"
#include "../Headers/funcoes_padrao.h"

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade
 * * @param perguntas Array de ponteiros para Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 */
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel) {
    
    //Variaveis
    Pergunta *filtradas[total]; // VLA - C99. Cuidado com pilhas muito grandes.
    int count = 0;

    //Copia as perguntas do nível
    for (int i = 0; i < total; i++) {
        if ((int)perguntas[i].nivel == nivel) {
            filtradas[count++] = &perguntas[i];
        }//if
    }//for

    if (count == 0) return NULL;

    //Sorteia a Pergunta
    // srand((unsigned)time(NULL)); // Não chame srand() dentro de um loop, apenas uma vez no main().
    int idx = rand() % count;
    return filtradas[idx];
}//sorteiaPorNivel

/**
 * brief loop interno que mantem o jogo rodando
 * * @param perguntas Array de ponteiros para Pergunta
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
            Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel); // Captura o retorno em uma nova variável
            if (pergunta_sorteada == NULL) {
                printf("Nenhuma pergunta encontrada para o nivel %d!\n", nivel);
                return; // Encerra ou lida com a falta de perguntas
            }

            //Mostra Pergunta
            mostraPergunta(pergunta_sorteada);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == pergunta_sorteada->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso(&(ProgressoJogador){.nome = "Jogador", .acertos = acerto}, "progresso.bin");
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while

        /*====== Perguntas do Nivel Facil=====*/
        while (acerto < 4){
            Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
            if (pergunta_sorteada == NULL) {
                printf("Nenhuma pergunta encontrada para o nivel %d!\n", nivel);
                return;
            }

            //Mostra Pergunta
            mostraPergunta(pergunta_sorteada);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == pergunta_sorteada->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso("progresso.bin", &(ProgressoJogador){.nome = "Jogador", .acertos = acerto});
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while

        /*====== Perguntas do Nivel Medio=====*/
        while (acerto < 8){
            Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
            if (pergunta_sorteada == NULL) {
                printf("Nenhuma pergunta encontrada para o nivel %d!\n", nivel);
                return;
            }

            //Mostra Pergunta
            mostraPergunta(pergunta_sorteada);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == pergunta_sorteada->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso("progresso.bin", &(ProgressoJogador){.nome = "Jogador", .acertos = acerto});
                return; // Encerra o jogo se errar
            }//if e else

            nivel++;
        }//while

        /*====== Perguntas do Nivel Dificil=====*/
        while (acerto < 12){
            Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
            if (pergunta_sorteada == NULL) {
                printf("Nenhuma pergunta encontrada para o nivel %d!\n", nivel);
                return;
            }

            //Mostra Pergunta
            mostraPergunta(pergunta_sorteada);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == pergunta_sorteada->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso("progresso.bin", &(ProgressoJogador){.nome = "Jogador", .acertos = acerto});
                return; // Encerra o jogo se errar
            }//if e else

            nivel++;
        }//while

        /*====== Perguntas do Nivel Muito Dificil=====*/
        while (acerto < 16){
            Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
            if (pergunta_sorteada == NULL) {
                printf("Nenhuma pergunta encontrada para o nivel %d!\n", nivel);
                return;
            }
            
            if (acerto == 15){
                perguntaDoMilhao(pergunta_sorteada); // Passa o ponteiro para a pergunta sorteada
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso("progresso.bin", &(ProgressoJogador){.nome = "Jogador", .acertos = acerto});
                return;
            }//if

            //Mostra Pergunta
            mostraPergunta(pergunta_sorteada);
            printf("Digite a letra da alternativa correta: ");
            limpaBuffer();
            scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
            resposta = toupper(resposta);
            if (resposta == pergunta_sorteada->correta) {
                printf("\033[0;32mCorreto!\033[0m\n");
                acerto++;
            } else {
                printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);
                char nome[50];
                printf("Digite seu nome para salvar o progresso: ");
                scanf("%49s", nome);

                ProgressoJogador progresso;
                strcpy(progresso.nome, nome); // Copia o nome do jogador
                progresso.acertos = acerto; // Salva o número de acertos
                salvaProgresso("progresso.bin", &(ProgressoJogador){.nome = "Jogador", .acertos = acerto});
                return; // Encerra o jogo se errar
            }//if e else
            
            nivel++;
        }//while
    }//while
}//jogoAcontece


/**
 * @brief Função que exibe a pergunta do Milhão 
 * @param perguntas Ponteiro para a Pergunta do Milhão
 */
void perguntaDoMilhao(Pergunta *pergunta_do_milhao) { // Alterado o parâmetro para Pergunta*
    //Variaveis
    char resposta;

    //Mostra Pergunta
    printf("\033[1;33mParabéns! Você chegou à pergunta do milhão!\033[0m\n");
    mostraPergunta(pergunta_do_milhao); // Usa o ponteiro passado
    printf("Digite a letra da alternativa correta: ");
    limpaBuffer();
    scanf(" %c", &resposta); // O espaço ignora espaços e quebras de linha
    resposta = toupper(resposta);
    
    if (resposta == pergunta_do_milhao->correta) {
        printf("\033[1;32mParabéns! Você ganhou o jogo!\033[0m\n");
    } else {
        printf("\033[1;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_do_milhao->correta);
    }//if e else
} //perguntaDoMilhao

/**
 * @brief Função que libera todas as perguntas alocadas e encerra o jogo
 * @param perguntas Array de ponteiros para Pergunta
 * @param total_perguntas Número total de perguntas para liberar
 */
void liberaRecursos(Pergunta *perguntas, int total_perguntas) { // Corrigido para 2 parâmetros
    for (int i = 0; i < total_perguntas; i++) { // Loop correto usando total_perguntas
        if (perguntas[i].enunciado != NULL) { // Verifica se não é NULL antes de liberar
            free(perguntas[i].enunciado);
            perguntas[i].enunciado = NULL;
        }
        for (int j = 0; j < 4; j++) {
            if (perguntas[i].alternativas[j].texto != NULL) { // Verifica se não é NULL
                free(perguntas[i].alternativas[j].texto);
                perguntas[i].alternativas[j].texto = NULL;
            }
        }
    }
    if (perguntas != NULL) { // Verifica se o array de perguntas não é NULL antes de liberar
        free(perguntas);
        perguntas = NULL;
    }
} //liberaRecursos

/**
 * @brief Função que salva o progresso do jogador em um arquivo binário
 * @param arquivo Nome do arquivo onde o progresso será salvo
 * @param progresso Ponteiro para a estrutura ProgressoJogador que contém os dados do
 */
void salvaProgresso(ProgressoJogador *progresso, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "ab"); // "ab" para adicionar ao final
    if (!fp) {
        printf("Erro ao abrir arquivo para salvar progresso!\n");
        return;
    }
    fwrite(progresso, sizeof(ProgressoJogador), 1, fp);
    fclose(fp);
}