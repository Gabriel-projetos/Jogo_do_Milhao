#include <stdlib.h>
#include <stdio.h>

/*Estruturas*/
typedef struct{
     char enunciado[185];
     Alternativa alternativas;
     char correta;
     Dificuldade dificuldade;
}Pergunta;

typedef struct{
     char letra;
     char texto[40];
}Alternativa;

typedef enum{
     MF, //2, 3
     F, //2
     M, // 4
     D, // 4
     MD // 3
}Dificuldade;

int main(){

     //Variaveis
     
     return 0;
}//main