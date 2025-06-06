#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/pergunta.h"
#include "../headers/jogo.h"

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade
 * 
 * @param perguntas Array de ponteiros para Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 */
Pergunta* sorteiaPorNivel(Pergunta **perguntas, int total, int nivel) {
    
    //Variaveis
    Pergunta *filtradas[total];
    int count = 0;

    //Copia as perguntas do nível
    for (int i = 0; i < total; i++) {
        if ((int)perguntas[i]->nivel == nivel) {
            filtradas[count++] = perguntas[i];
        }//if
    }//for

    if (count == 0) return NULL;

    //Sorteia a Pergunta
    srand((unsigned)time(NULL));
    int idx = rand() % count;
    return filtradas[idx];
}//sorteiaPorNivel