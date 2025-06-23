#include "../Headers/leitor_csv.h"
#include "../Headers/pergunta.h"       
#include "../Headers/funcoes_padrao.h"  


// Função para remover espaços no início e fim de uma string
char* trim(char *str) {
    char *end;

    // Remove espaços à esquerda
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str; // string vazia

    // Remove espaços à direita
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Define fim da string
    *(end + 1) = '\0';
    return str;
}

// Função principal para carregar perguntas de um CSV
Pergunta* carregaPerguntasDeCSV(const char *nome_arquivo, int *total) {
    //Abre o arquivo CSV
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        *total = 0;
        return NULL;
    }//if


    int capacidade = 15;
    int count = 0;
    Pergunta *perguntas = malloc(capacidade * sizeof(Pergunta));
    if (!perguntas) {
        printf("Erro ao alocar memória\n");
        fclose(file);
        *total = 0;
        return NULL;
    }//if

    char linha[1024];

    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), file);

    while (fgets(linha, sizeof(linha), file)) {
        if (count >= capacidade) {
            capacidade *= 2;
            Pergunta *temp = realloc(perguntas, capacidade * sizeof(Pergunta));
            if (!temp) {
                printf("Erro ao realocar memória\n");
                fclose(file);
                // Limpa strings já alocadas das perguntas carregadas
                for (int i = 0; i < count; i++) {
                    free(perguntas[i].enunciado);
                    for (int j = 0; j < 4; j++) free(perguntas[i].alternativas[j].texto);
                }//for
                free(perguntas);
                *total = 0;
                return NULL;
            }//if interno
            perguntas = temp;
        }//if externo
        
        // Tokeniza linha pelo separador ';'
        char *token = strtok(linha, ";");
        if (!token) continue;

        // Enunciado
        char *enunciado = strdup(trim(token));
        if (!enunciado){
            printf("Erro ao alocar enunciado\n");
            continue;
        }
        

        // Alternativas A, B, C, D
        char *alternativas[4];
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, ";");
            alternativas[i] = token ? strdup(trim(token)) : strdup("");
             if (!alternativas[i]) {
                printf("Erro ao alocar alternativa\n");
                // Liberar strings alocadas até aqui...
                for (int k = 0; k < i; k++) free(alternativas[k]);
                free(enunciado);
                continue;
            }
        }

        // Letra da alternativa correta
        token = strtok(NULL, ";");
        char correta = 'A';
        if(token && strlen(token) > 0){
            char *ptr = trim(token);
            correta = toupper(ptr[0]);
        }//if

        // Nível de dificuldade
        token = strtok(NULL, ";");
        int nivel = (token) ? atoi(token) : 1;
        if(nivel < 1 || nivel > 5) nivel = 1;

        // Preenche a struct
        Pergunta *p = &perguntas[count];
        p->enunciado = enunciado;

        char letras[] = {'A', 'B', 'C', 'D'};
        for (int i = 0; i < 4; i++) {
            p->alternativas[i].letra = letras[i];
            p->alternativas[i].texto = alternativas[i];
            converteMaiscula(p->alternativas[i].texto);
        }

        p->correta = correta;
        p->nivel = (Dificuldade) nivel;

        count++;
    }

    fclose(file);
    *total = count;
    return perguntas;
}
