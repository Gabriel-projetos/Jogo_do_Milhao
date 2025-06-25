#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"ranking.h"

//------------------------------------------------------------------------------------------
// Métodos do Ranking
//------------------------------------------------------------------------------------------

char nome_arquivo[30] = "data/ranking.dat";

Ranking* lerRanking(){
    Ranking *ranking = calloc(LEADER_SIZE, sizeof(Ranking));

    FILE* arquivo = fopen(nome_arquivo, "rb");
    
    if (!arquivo){
        printf("Ranking não encontrado. Criando um vazio\n");
        for (size_t i = 0; i < LEADER_SIZE; i++){
            ranking[i].name[0] = '\0';
            ranking[i].score = -1;
        }
    } else {
        fread(ranking, sizeof(Ranking), LEADER_SIZE, arquivo);
        fclose(arquivo);
    }
    return ranking;
}

void salvarRanking(Ranking *ranking){

    FILE* arquivo = fopen(nome_arquivo, "wb");
    
    if (arquivo == NULL) {
        perror("Erro ao salvar o Ranking");
    } else {
        fwrite(ranking, sizeof(Ranking), LEADER_SIZE, arquivo);
        fclose(arquivo);
    }

    return;
}

int adicionarRanking(Ranking* ranking, char* nome, int pontuacao){

    int pos = LEADER_SIZE - 1;
    Ranking novoRanking;
    strcpy(novoRanking.name, nome);
    novoRanking.score = pontuacao;

    while(ranking[pos].score <= novoRanking.score && pos >= 0){
        ranking[pos] = ranking[pos - 1];
        pos--;
    }
    if(pos != LEADER_SIZE - 1){
        ranking[pos + 1] = novoRanking;
    }

    return pos + 1;
}

void imprimirRanking(Ranking *ranking){

    printf("\n|  # | Nome                 | Pontos |\n");
    for (int i = 0; i < LEADER_SIZE; i++){
        printf("| %2d | %-20s | %6d |\n", i + 1, ranking[i].name, ranking[i].score);
    }
    return;
}

int menorPontuacao(Ranking* ranking){
    return ranking[LEADER_SIZE - 1].score;
}

// int main(){
//     Ranking* ranking = lerRanking();

//     imprimirRanking(ranking);
//     adicionarRanking(ranking, "Muriel", 50);
//     adicionarRanking(ranking, "Mantova", 70);
//     adicionarRanking(ranking, "Luizao", 60);
//     adicionarRanking(ranking, "Marcelo", 90);
//     adicionarRanking(ranking, "Adalberto", 30);
//     adicionarRanking(ranking, "Maurichan", 50);
//     adicionarRanking(ranking, "Fabiao", 70);
//     adicionarRanking(ranking, "Wendel", 60);
//     adicionarRanking(ranking, "Wagner", 90);
//     adicionarRanking(ranking, "Andre do Mato", 30);

//     salvarRanking(ranking);
    
//     imprimirRanking(ranking);

//     return 0;
// }
