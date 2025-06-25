#ifndef JOGO_H
#define JOGO_H

#include "pergunta.h"

typedef struct {
    int nome[50]; // Nível de dificuldade (1 a 5)
    int acertos; // Número de acertos
} ProgressoJogador;

Pergunta* sorteiaPergunta(Pergunta *perguntas, int total);
void embaralhaPerguntas(Pergunta **originais, Pergunta **embaralhadas, int total);
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel);
void jogoAcontece(Pergunta *perguntas, int total); 
void perguntaDoMilhao(Pergunta *perguntas); 
void salvaProgresso(ProgressoJogador *progresso, const char *nome_arquivo);

#endif // JOGO_H
