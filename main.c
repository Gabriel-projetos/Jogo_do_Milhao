#include "raylib.h"
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

//Função do Menu Inicial
int menuInicial(){
    int op;
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
    scanf("%d", &op);
    menu_opcao = (Opcao)op;
    return menu_opcao;
}//menuInicial

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);

    //Variaveis
    Pergunta *perguntas = NULL;
    int total_perguntas = 0;
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
            salvaPerguntaNoCSV("questoes.csv", &perguntas[total_perguntas - 1]);
            menu_opcao = menuInicial();
            break;
        case LISTAR:
            listaPerguntas(perguntas, total_perguntas);
            menu_opcao = menuInicial();
            break;
        case PESQUISAR:
            pesquisaPergunta(perguntas, total_perguntas);
            menu_opcao = menuInicial();
            break;
        case ALTERAR:
            alterarPergunta(perguntas, total_perguntas);
            salvaPerguntaNoCSV("questoes.csv", &perguntas[total_perguntas]);
            menu_opcao = menuInicial();
            break;
        case EXCLUIR:
            excluirPergunta(&perguntas, &total_perguntas);
            menu_opcao = menuInicial();
            break;
        case JOGAR:
            printf("Iniciando o jogo...\n");
            jogoAcontece(perguntas, total_perguntas);
            printf("Fim do jogo!\n");
            liberaRecursos(perguntas, total_perguntas);
            break;
        case SAIR:
                printf("Saindo do programa...\n");
                liberaRecursos(perguntas, total_perguntas);
                return 0;
            default:
                printf("Opção inválida!\n");
                menu_opcao = menuInicial();
                break;
        }

        return 0;
    }
