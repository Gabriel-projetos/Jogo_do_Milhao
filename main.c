#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/pergunta.h"
#include "headers/leitor_csv.h"
#include "headers/funcoes_padrao.h"
#include "headers/menu_inicial.h"
#include "headers/jogo.h"

int menuInicial(){
    Opcao menu_opcao;
    //Menu inicial
    printf("Bem vindo ao Jogo do Milhão!\n");
    printf("Escolha uma opção:\n");
    printf("1 - Inserir pergunta\n");
    printf("2 - Listar perguntas\n");
    printf("3 - Pesquisar pergunta\n");
    printf("4 - Alterar pergunta\n");
    printf("5 - Excluir pergunta\n");
    printf("6 - Jogar\n");
    printf("0 - Sair\n");
    scanf("%d", &menu_opcao);
}//menuInicial

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);

    Pergunta *perguntas = NULL;
    int op, total_perguntas = 0;
    Opcao menu_opcao;

    // Carrega perguntas do CSV
    perguntas = carregaPerguntasDeCSV("questoes.csv", &total_perguntas);
    if (perguntas == NULL) {
        printf("Erro ao carregar perguntas do arquivo CSV!\n");
        return 1;
    }
    printf("Foram carregadas %d perguntas.\n", total_perguntas);

    //Menu Inicial
    menu_opcao = menuInicial();
    switch(menu_opcao){
        case INSERIR:
            recebePergunta(&perguntas, &total_perguntas);
            salvaPerguntaNoCSV("questoes.csv", perguntas[total_perguntas - 1]);
            break;
        case LISTAR:
            listaPerguntas(&perguntas, total_perguntas);
            break;
        case PESQUISAR:
            pesquisaPergunta(&perguntas, total_perguntas);
            break;
        case ALTERAR:
            alterarPergunta(&perguntas, total_perguntas);
            salvaPerguntaNoCSV("questoes.csv", perguntas[total_perguntas]);
            break;
        case EXCLUIR:
            excluirPergunta(&perguntas, &total_perguntas);
            salvaTodasPerguntasNoCSV("questoes.csv", perguntas, total_perguntas);
            break;
        case JOGAR:
            printf("Iniciando o jogo...\n");
                for (int nivel = 1; nivel <= 5; nivel++) {
                    Pergunta *p = sorteiaPorNivel(&perguntas, total_perguntas, nivel);
                    if (p != NULL) {
                        mostraPergunta(p);
                        printf("Digite a letra da alternativa correta: ");
                        char resposta;
                        scanf(" %c", &resposta);
                        if (toupper(resposta) == toupper(p->correta)) {
                            printf("\033[0;32mCorreto!\033[0m\n");
                        } else {
                            printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", p->correta);
                            break;
                        }
                    } else {
                        printf("Sem pergunta disponível para o nível %d.\n", nivel);
                    }
                }
                break;
        case SAIR:
                printf("Saindo do programa...\n");
                // Liberar memória
                for (int i = 0; i < total_perguntas; i++) {
                    free(perguntas[i].enunciado);
                    for (int j = 0; j < 4; j++) {
                        free(perguntas[i].alternativas[j].texto);
                    }
                }
                free(perguntas);
                return 0;
            default:
                printf("Opção inválida!\n");
                break;
        }

        return 0;
    }