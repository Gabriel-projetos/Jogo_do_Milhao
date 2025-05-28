#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/pergunta.h"

/**
 * @brief Função para contar o número de linhas em um arquivo CSV
 * 
 * @param filename Nome do arquivo CSV
 * @return Número de linhas no arquivo
 */
int contarLinhasCSV(char *filename){
    FILE *arq = fopen("questoes.csv", "r");
    if (arq == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }//if

    int cont = 0;
    char linha[1024];
    while (fgets(linha, sizeof(linha), arq) != NULL){
        if(linha[0] != '\n' && linha[0] != '#')
            cont++;
    }//while
}//contarLinhasCSV

/**
 * @brief Sorteia uma pergunta do arquivo CSV
 * 
 * @param questoes Nome do arquivo CSV
 * @param pergunta_sorteada Ponteiro que armazena a pergunta
 * @return 0 em caso sucesso, -1 em caso de erro
 */
int sorteiaPergunta(char *questoes, Pergunta *pergunta_sorteada){
    
    //Variaveis
    int num_linha, linha_sorteada, linha_atual=0;
    FILE *arq = fopen("questoes.csv", "r");
    char linha[1024], *token, *resto;

    if(arq == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }//if

    while(fgets(linha, sizeof(linha), arq) != NULL){

        num_linha = contarLinhasCSV(questoes);
        if(num_linha <= 0){
            printf("Nenhuma pergunta disponivel\n");
        return -1;
    }//if

        rand(time(NULL));
        int linha_sorteada = rand()% num_linha;

        // Extrai a pergunta sorteada
        token = strtok_r(resto, ";", &resto);
        if (token != NULL) {
            // Remove o caractere de nova linha, se presente
            size_t len = strlen(token);
            if (len > 0 && token[len - 1] == '\n') {
                token[len - 1] = '\0';
            }//if
            strncpy(pergunta_sorteada->enunciado, token, sizeof(pergunta_sorteada->enunciado) - 1);
            pergunta_sorteada->enunciado[sizeof(pergunta_sorteada->enunciado) - 1] = '\0';
        } else {
            fprintf(stderr, "Erro ao extrair a pergunta da linha %d.\n", linha_sorteada + 1);
            fclose(arq);
            return -1;
        }//if else

        //Extrai as alternativas
        for(int i = 0; i < 4; i++){
            token = strtok_r(resto, ";", &resto);
            if(token != NULL){
                size_t len = strlen(token);
                if(len > 0 && token[len - 1] == '\n'){
                    token[len -1] = '\0';
                }//if
                strncpy(pergunta_sorteada->alternativas[i].texto, token, sizeof(pergunta_sorteada->alternativas[i].texto) - 1);
                pergunta_sorteada->alternativas[i].texto[sizeof(pergunta_sorteada->alternativas[i].texto) - 1] = '\0';
            }//if
        }//for

        //Extrai a correta
        token = strtok_r(resto, ";", &resto);
        if(token != NULL){
            size_t len = strlen(token);
            if(len > 0 && token[len - 1] == '\n'){
                token[len - 1] = '\0';
                strncpy(pergunta_sorteada->correta, token, sizeof(pergunta_sorteada->correta) - 1);
                pergunta_sorteada->correta = token[0];
            }//if
        }//if
        
        //Extrai dificuldade
        token = strtok_r(resto, ";", &resto);
        if(token != NULL){
            size_t len = strlen(token);
            if(len > 0 && token[len - 1] == '\n'){
                token[len - 1] = '\0';
            }//if

            pergunta_sorteada->nivel = atoi(token);
        }//if
    }//while
    linha_atual++;

    fclose(arq);
}//sorteiaPergunta

// Sortear a pergunta por nível de dificuldade
Pergunta* sorteiaPorNivel(Pergunta **perguntas, int total, int nivel) {
    Pergunta *filtradas[total];
    int count = 0;

    for (int i = 0; i < total; i++) {
        if ((int)perguntas[i]->nivel == nivel) {
            filtradas[count++] = perguntas[i];
        }
    }

    if (count == 0) return NULL;

    srand((unsigned)time(NULL));
    int idx = rand() % count;
    return filtradas[idx];
}