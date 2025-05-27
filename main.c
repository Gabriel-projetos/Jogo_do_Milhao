#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/pergunta.h"
#include "headers/leitor_csv.h"
#include "headers/funcoes_padrao.h"

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);

    Pergunta *perguntas = NULL;
    int op, total_perguntas = 0;

    // Carrega perguntas do CSV
    perguntas = carregaPerguntasDeCSV("questoes.csv", &total_perguntas);
    if (perguntas == NULL) {
        printf("Erro ao carregar perguntas do arquivo CSV!\n");
        return 1;
    }
    printf("Foram carregadas %d perguntas.\n", total_perguntas);

    // Inserir nova pergunta
    printf("\nDeseja inserir uma nova pergunta?\n1 - Sim\n2 - Não\n");
    scanf("%d", &op);
    getchar(); // limpar buffer
    switch (op) {
        case 1:
            recebePergunta(&perguntas, &total_perguntas);
            break;
        case 2:
            printf("Continuando programa!\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }

    // Listar perguntas
    printf("\nDeseja listar as perguntas?\n1 - Sim\n2 - Não\n");
    scanf("%d", &op);
    limpaBuffer();
    switch (op) {
        case 1:
            listaPerguntas(perguntas, total_perguntas);
            break;
        case 2:
            printf("Continuando programa!\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }

    // Pesquisar perguntas
    printf("\nDeseja pesquisar uma pergunta?\n1 - Sim\n2 - Não\n");
    scanf("%d", &op);
    limpaBuffer();
    switch (op) {
        case 1:
            pesquisaPergunta(perguntas, total_perguntas);
            break;
        case 2:
            printf("Continuando programa!\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }

    // Alterar pergunta
    printf("\nDeseja trocar uma pergunta?\n1 - Sim\n2 - Não\n");
    scanf("%d", &op);
    limpaBuffer();
    switch (op) {
        case 1:
            alterarPergunta(perguntas, total_perguntas);
            break;
        case 2:
            printf("Continuando programa!\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }

    // Excluir pergunta
    printf("\nDeseja excluir uma pergunta?\n1 - Sim\n2 - Não\n");
    scanf("%d", &op);
    limpaBuffer();
    switch (op) {
        case 1:
            excluirPergunta(&perguntas, &total_perguntas);
            break;
        case 2:
            printf("Continuando programa!\n");
            break;
        default:
            printf("Opção inválida!\n");
            break;
    }

    // Liberar memória
    for (int i = 0; i < total_perguntas; i++) {
        free(perguntas[i].enunciado);
        for (int j = 0; j < 4; j++) {
            free(perguntas[i].alternativas[j].texto);
        }
    }
    free(perguntas);

    return 0;
}
