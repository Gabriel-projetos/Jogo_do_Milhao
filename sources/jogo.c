#include <time.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h>
#include "pergunta.h"
#include "jogo.h"
#include "funcoes_padrao.h"

typedef struct {
    char nome[50];
    int acertos;
} ProgressoJogador;

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade, evitando repetidas.
 * @param perguntas Array de Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 * @return Ponteiro para a Pergunta sorteada ou NULL se não encontrar
 */
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel) {
    int tentativas = 0;
    while (tentativas < 100) {
        int idx = rand() % total;
        if (perguntas[idx].nivel == nivel && perguntas[idx].ja_foi_usada == 0) {
            perguntas[idx].ja_foi_usada = 1;
            return &perguntas[idx];
        }
        tentativas++;
    }
    return NULL;
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
    int nivel = 1;
    int acertosNoNivel = 0;
    char resposta;

    // Seleciona a pergunta do milhão (nível 5) antecipadamente
    Pergunta *pergunta_milhao = NULL;
    for (int i = 0; i < total; i++) {
        if (perguntas[i].nivel == 5 && perguntas[i].ja_foi_usada == 0) {
            pergunta_milhao = &perguntas[i];
            perguntas[i].ja_foi_usada = 1;
            break;
        }
    }
    if (pergunta_milhao == NULL) {
        printf("Erro: Nenhuma pergunta do milhão disponível.\n");
        return;
    }

    while (1) {
        Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
        if (pergunta_sorteada == NULL) {
            // Não tem mais perguntas no nível atual, sobe de nível
            nivel++;
            acertosNoNivel = 0;
            if (nivel == 5) break; // Chegou no nível do milhão
            continue;
        }

        mostraPergunta(pergunta_sorteada);
        printf("Digite a letra da alternativa correta: ");
        limpaBuffer();
        scanf(" %c", &resposta);
        resposta = toupper(resposta);

        if (resposta == pergunta_sorteada->correta) {
            printf("\033[0;32mCorreto!\033[0m\n");
            acertosNoNivel++;
            if (acertosNoNivel == 3) {
                nivel++;
                acertosNoNivel = 0;
                if (nivel == 5) break; // Chegou no nível do milhão
            }
        } else {
            printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta);

            ProgressoJogador progresso;
            printf("Digite seu nome para salvar o progresso: ");
            scanf("%49s", progresso.nome);
            progresso.acertos = (nivel - 1) * 3 + acertosNoNivel; // total de acertos até aqui

            salvaProgresso(&progresso, "progresso.bin");
            return; // encerra o jogo ao errar
        }
    }

    // Pergunta do Milhão
    perguntaDoMilhao(pergunta_milhao);
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

