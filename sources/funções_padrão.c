#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>      // para toupper()
#include <string.h>
#include "../headers/pergunta.h"

char *leTextoDinamico(){
     char *texto = NULL;
     int c;
     size_t tamanho = 0;

     while ((c = getchar()) != '\n' && c != EOF) {
          texto = realloc(texto, tamanho + 2);
          if (texto == NULL) {
               printf("Erro ao alocar memória!\n");
               exit(1);
          }
          texto[tamanho++] = c;
     }

     if(texto) texto[tamanho] = '\0';
     return texto;
}

void limpaBuffer(void){
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}

void letexto(char texto[], int tamanho){
  fgets(texto, tamanho, stdin);
  texto[strcspn(texto, "\n")] = '\0';
}

int* alocaVetor(int n){
    if(n <= 0){
        return NULL;
    }
    int *vetor = (int*) malloc(n * sizeof(int));
    if(vetor == NULL){
        printf("Erro ao alocar o vetor!");
        exit(1);
    }
    return vetor;
}

void converteMaiscula(char texto[]){
  for(int i = 0; texto[i] != 0; i++){
         texto[i] = toupper(texto[i]);
  }
}
