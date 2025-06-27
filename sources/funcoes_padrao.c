#include "pergunta.h"

int g_current_display_question_idx = 0;

// Função para ler uma linha de texto dinamicamente (até ENTER)
char *leTextoDinamico(){
     char *texto = NULL;
     int c;
     size_t tamanho = 0;

     while ((c = getchar()) != '\n' && c != EOF) {
      // Realoca +2 para guardar o caractere atual + '\0'
        char *temp = realloc(texto, tamanho + 2);
          if (temp == NULL) {
              free(texto);
              printf("Erro ao alocar memória!\n");
              exit(1);
          }
          texto = temp;
          texto[tamanho++] = c;
     }

     if(texto) texto[tamanho] = '\0';// Finaliza string
     else {  // Se nada foi digitado, cria string vazia
        texto = malloc(1);
        if (!texto) {
            printf("Erro ao alocar memória!\n");
            exit(1);
        }
        texto[0] = '\0';
    }
     return texto;
}

// Limpa o buffer do stdin até o próximo '\n' ou EOF
void limpaBuffer(void){
  int c;
  while((c = getchar()) != '\n' && c != EOF);
}

// Lê uma string usando fgets (máximo tamanho-1 caracteres), retira '\n' final
void letexto(char texto[], int tamanho){
  if (fgets(texto, tamanho, stdin)){
      texto[strcspn(texto, "\n")] = '\0';  // Remove '\n'
  } else{
    texto[0] = '\0'; // caso fgets falhe
  }
  
}

// Aloca vetor de inteiros de tamanho n, com checagem de erro
int* alocaVetor(int n){
    if(n <= 0) return NULL;

    int *vetor = (int*) malloc(n * sizeof(int));
    if(!vetor){
        printf("Erro ao alocar o vetor!");
        exit(1);
    }
    return vetor;
}

// Converte string para maiúsculas in-place
void converteMaiscula(char texto[]){
  for(int i = 0; texto[i] != 0; i++){
         texto[i] = toupper((unsigned char)texto[i]);
  }

}
