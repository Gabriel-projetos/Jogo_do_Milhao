#ifndef LEADERSYSTEM_H
#define LEADERSYSTEM_H

#define LEADER_SIZE 10
#define MAX_LENGTH 16

typedef struct{
    char name[MAX_LENGTH];
    int score;
} Ranking;

//------------------------------------------------------------------------------------------
// Métodos do Ranking
//------------------------------------------------------------------------------------------
Ranking* lerRanking();
void salvarRanking(Ranking *ranking);
int adicionarRanking(Ranking* ranking, char* nome, int pontuacao);
void imprimirRanking(Ranking *ranking);
int menorPontuacao(Ranking *ranking);

#endif