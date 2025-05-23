#include <stdio.h>
#include <stdlib.h>
#include "../Headers/pergunta.h"

char *leTextoDinamico(){
     char *texto = NULL;
     int c;
     size_t tamanho = 0;

     while ((c = getchar()) != '\n' && c != EOF) {
          texto = realloc(texto, tamanho + 2);  // +1 para caractere e +1 para '\0'
          if (texto == NULL) {
               printf("Erro ao alocar memória!\n");
               exit(1);
          }//if
          texto[tamanho++] = c;
     }//while

     if(texto) texto[tamanho] = '\0'; // Adiciona o caractere nulo no final
}//leTextoDinamico

void limpaBuffer(void){
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}//limpaBuffer

void letexto(char texto[], int tamanho){
  fgets(texto, tamanho, stdin);
  texto[strcspn(texto, "\n")] = '\0';
}//letexto

int* alocaVetor(int n){
    int *vetor;
    if(n <= 0){
        return NULL;
    }else{
        vetor = (int*) malloc(n * sizeof(int));
        if(vetor == NULL){
            printf("Erro ao alocar o vetor!");
            exit(1);
        }//If

        return (int*) malloc(n * sizeof(int));
    }//ifelse
}//alocaVetor

void converteMaiscula(char texto[]){
  for(int i = 0; texto[i] != 0; i++){
         texto[i] = toupper(texto[i]);
  }//For
}//converteMaiscula