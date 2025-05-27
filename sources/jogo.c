#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/pergunta.h"

// Aleatorizar uma pergunta
Pergunta* sorteiaPergunta(Pergunta **perguntas, int total) {
    if (total == 0) return NULL;
    srand((unsigned)time(NULL));
    int indice = rand() % total;
    return perguntas[indice];
}

// Aleatorizar várias perguntas diferentes
void embaralhaPerguntas(Pergunta **originais, Pergunta **embaralhadas, int total) {
    srand((unsigned)time(NULL));
    int usados[total];
    for (int i = 0; i < total; i++) usados[i] = 0;

    for (int i = 0; i < total; i++) {
        int indice;
        do {
            indice = rand() % total;
        } while (usados[indice]);
        usados[indice] = 1;
        embaralhadas[i] = originais[indice];
    }
}

// Sortear a pergunta por nível de dificuldade
Pergunta* sorteiaPorNivel(Pergunta **perguntas, int total, int nivel) {
    Pergunta *filtradas[total];
    int count = 0;

    for (int i = 0; i < total; i++) {
        if (perguntas[i]->nivel == nivel) {
            filtradas[count++] = perguntas[i];
        }
    }

    if (count == 0) return NULL;

    srand((unsigned)time(NULL));
    int idx = rand() % count;
    return filtradas[idx];
}