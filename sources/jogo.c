#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h>
#include "pergunta.h"
#include "jogo.h"
#include "funcoes_padrao.h"

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade, evitando repetidas.
 * @param perguntas Array de Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 * @return Ponteiro para a Pergunta sorteada ou NULL se não encontrar
 */
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel) {
    int indices_validos[total];
    int contador = 0;

    // Coleta os índices de perguntas disponíveis do nível desejado
    for (int i = 0; i < total; i++) {
        if (perguntas[i].nivel == nivel && perguntas[i].ja_foi_usada == 0) {
            indices_validos[contador++] = i;
        }
    }

    if (contador == 0) return NULL; // Nenhuma pergunta disponível

    int sorteado = indices_validos[rand() % contador];
    perguntas[sorteado].ja_foi_usada = 1;
    return &perguntas[sorteado];
}



/**
 * @brief Função que salva o progresso do jogador em um arquivo binário
 * @param progresso Ponteiro para a estrutura ProgressoJogador que contém os dados do jogador
 * @param nome_arquivo Nome do arquivo onde o progresso será salvo
 */
void salvaProgresso(ProgressoJogador *progresso, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "ab"); // abre para acrescentar no final
    if (!fp) {
        printf("Erro ao abrir arquivo para salvar progresso!\n");
        return;
    }
    fwrite(progresso, sizeof(ProgressoJogador), 1, fp);
    fclose(fp);
}

/**
 * @brief Loop principal do jogo que faz a lógica de perguntas e níveis.
 * @param perguntas Array de Pergunta
 * @param total Número total de perguntas
 */
void jogoAcontece(Pergunta perguntas[], int total) {
    int pergunta_atual = 1;
    char resposta;
    int acertos = 0;

    while (pergunta_atual <= 15) {
        int nivel;

        // Define o nível com base na questão atual
        if (pergunta_atual <= 2)
            nivel = 1; // Muito fácil
        else if (pergunta_atual <= 4)
            nivel = 2; // Fácil
        else if (pergunta_atual <= 8)
            nivel = 3; // Médio
        else if (pergunta_atual <= 12)
            nivel = 4; // Difícil
        else
            nivel = 5; // Muito difícil

        Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
        if (pergunta_sorteada == NULL) {
            printf("Não há mais perguntas disponíveis para o nível %d.\n", nivel);
            break;
        }

        mostraPergunta(pergunta_sorteada);
        printf("Digite a letra da alternativa correta: ");
        limpaBuffer();
        scanf(" %c", &resposta);
        resposta = toupper(resposta);

        if (resposta == pergunta_sorteada->correta) {
            printf("\033[0;32mCorreto!\033[0m\n");
            acertos++;
            pergunta_atual++;
        } else {
            printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);

            ProgressoJogador progresso;
            printf("Digite seu nome para salvar o progresso: ");
            scanf("%49s", progresso.nome);
            progresso.acertos = acertos;
            salvaProgresso(&progresso, "progresso.bin");
            return;
        }
    }

    if (pergunta_atual > 15) {
        printf("\033[1;32mParabéns! Você venceu o jogo!\033[0m\n");
        ProgressoJogador progresso;
        printf("Digite seu nome para salvar o progresso: ");
        scanf("%49s", progresso.nome);
        progresso.acertos = acertos;
        salvaProgresso(&progresso, "progresso.bin");
    }
}

/**
 * @brief Exibe a pergunta do milhão e trata resposta final
 * @param pergunta_do_milhao Ponteiro para a pergunta do milhão
 */
void perguntaDoMilhao(Pergunta *pergunta_do_milhao) {
    char resposta;
    printf("\033[1;33mParabéns! Você chegou à pergunta do milhão!\033[0m\n");
    mostraPergunta(pergunta_do_milhao);
    printf("Digite a letra da alternativa correta: ");
    limpaBuffer();
    scanf(" %c", &resposta);
    resposta = toupper(resposta);

    if (resposta == pergunta_do_milhao->correta) {
        printf("\033[1;32mParabéns! Você ganhou o jogo!\033[0m\n");
    } else {
        printf("\033[1;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_do_milhao->correta);
    }
}

/**
 * @brief Libera memória alocada para perguntas e alternativas
 * @param perguntas Array de Pergunta
 * @param total_perguntas Número total de perguntas para liberar
 */
void liberaRecursos(Pergunta *perguntas, int total_perguntas) {
    for (int i = 0; i < total_perguntas; i++) {
        if (perguntas[i].enunciado != NULL) {
            free(perguntas[i].enunciado);
            perguntas[i].enunciado = NULL;
        }
        for (int j = 0; j < 4; j++) {
            if (perguntas[i].alternativas[j].texto != NULL) {
                free(perguntas[i].alternativas[j].texto);
                perguntas[i].alternativas[j].texto = NULL;
            }
        }
    }
    if (perguntas != NULL) {
        free(perguntas);
        perguntas = NULL;
    }
}

