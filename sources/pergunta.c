#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/pergunta.h"
#include "../headers/funcoes_padrao.h"

// Recebe nova pergunta, realocando o vetor de Pergunta*
Pergunta** recebePergunta(Pergunta **perguntas, int *total) {
    Pergunta *nova = malloc(sizeof(Pergunta));
    if (!nova) {
        printf("Erro ao alocar memória para nova pergunta!\n");
        return perguntas;
    }

    // Recebe o enunciado
    printf("Digite o enunciado da questão:\n");
    limpaBuffer();
    nova->enunciado = leTextoDinamico();
    converteMaiscula(nova->enunciado);

    // Recebe alternativas
    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        nova->alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        nova->alternativas[i].texto = leTextoDinamico();
        converteMaiscula(nova->alternativas[i].texto);
    }

    // Recebe correta
    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &nova->correta);

    // Recebe dificuldade
    printf("Escolha a dificuldade da questão:\n");
    printf("1 - Muito Fácil\n2 - Fácil\n3 - Médio\n4 - Difícil\n5 - Muito Difícil\n");
    int nivel;
    scanf("%d", &nivel);
    nova->nivel = nivel;
    limpaBuffer();

    // Realoca o vetor de perguntas para guardar o novo ponteiro
    perguntas = realloc(perguntas, (*total + 1) * sizeof(Pergunta*));
    if (!perguntas) {
        printf("Erro ao realocar vetor de perguntas!\n");
        free(nova);
        return NULL;
    }
    perguntas[*total] = nova;
    (*total)++;

    printf("Pergunta adicionada com sucesso!\n");
    return perguntas;
}

// Mostra uma pergunta completa
void mostraPergunta(Pergunta *pergunta) {
    printf("%s\n", pergunta->enunciado);
    for (int i = 0; i < 4; i++) {
        printf("%c) %s\n", pergunta->alternativas[i].letra, pergunta->alternativas[i].texto);
    }
}

// Lista perguntas por nível
void listaPerguntas(Pergunta **perguntas, int total) {
    for (int nivel = 1; nivel <= 5; nivel++) {
        for (int i = 0; i < total; i++) {
            if ((int)perguntas[i]->nivel == nivel) {
                printf("\n");
                mostraPergunta(perguntas[i]);
            }
        }
    }
}

// Pesquisa pergunta pelo enunciado
void pesquisaPergunta(Pergunta **perguntas, int total) {
    char *pesquisa = leTextoDinamico();
    converteMaiscula(pesquisa);
    int encontrada = 0;
    for (int i = 0; i < total; i++) {
        if (strstr(perguntas[i]->enunciado, pesquisa)) {
            mostraPergunta(perguntas[i]);
            encontrada = 1;
        }
    }
    if (!encontrada) printf("Pergunta não encontrada!\n");
    free(pesquisa);
}

// Altera pergunta existente
void alterarPergunta(Pergunta **perguntas, int total) {
    int indice;
    printf("Qual índice da pergunta deseja alterar? ");
    scanf("%d", &indice);
    limpaBuffer();
    if (indice < 0 || indice >= total) {
        printf("Índice inválido!\n");
        return;
    }

    // Libera memória da pergunta atual
    free(perguntas[indice]->enunciado);
    for (int i = 0; i < 4; i++) free(perguntas[indice]->alternativas[i].texto);
    free(perguntas[indice]);

    // Recebe nova pergunta para substituir
    Pergunta *nova = malloc(sizeof(Pergunta));
    printf("Digite o enunciado da nova questão:\n");
    nova->enunciado = leTextoDinamico();
    converteMaiscula(nova->enunciado);

    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        nova->alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        nova->alternativas[i].texto = leTextoDinamico();
        converteMaiscula(nova->alternativas[i].texto);
    }

    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &nova->correta);

    int nivel;
    printf("Digite a dificuldade da pergunta (1-5):\n");
    scanf("%d", &nivel);
    nova->nivel = nivel;
    limpaBuffer();

    perguntas[indice] = nova;
    printf("Pergunta %d alterada com sucesso!\n", indice);
}

// Exclui pergunta e reorganiza vetor
void excluirPergunta(Pergunta **perguntas, int *total) {
    int indice;
    printf("Qual índice da pergunta deseja excluir (0 a %d)? ", *total - 1);
    scanf("%d", &indice);
    limpaBuffer();

    if (indice < 0 || indice >= *total) {
        printf("Índice inválido!\n");
        return;
    }

    free((*perguntas)[indice].enunciado);
    for (int i = 0; i < 4; i++) free((*perguntas)[indice].alternativas[i].texto);

    for (int i = indice; i < *total - 1; i++) {
        (*perguntas)[i] = (*perguntas)[i + 1];
    }

    *perguntas = realloc(*perguntas, (*total - 1) * sizeof(Pergunta*));
    (*total)--;
    printf("Pergunta excluída com sucesso!\n");
}

