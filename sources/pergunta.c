#include "pergunta.h"
#include "leitor_csv.h"
#include "funcoes_padrao.h" 
#include <string.h>
#include <stdlib.h> 

// Recebe nova pergunta, realocando o vetor de Pergunta*
void recebePergunta(Pergunta **perguntas, int *total) {
    // Tenta realocar o vetor para adicionar uma nova pergunta
    Pergunta *nova = realloc(*perguntas, (*total + 1) * sizeof(Pergunta));
    if (!nova) {
        printf("Erro ao alocar memória para nova pergunta!\n");
        return ;
    }

    *perguntas = nova; // Atualiza o ponteiro global para o novo vetor

    Pergunta *p = &((*perguntas)[*total]); // Pega o endereço da nova posição

    // Recebe o enunciado
    printf("Digite o enunciado da questão:\n");
    limpaBuffer(); // Limpa o buffer de entrada
    char *temp_enunciado = leTextoDinamico(); // Lê o texto dinamicamente
    // Copia o texto lido para o array fixo na struct e garante terminação nula
    strncpy(p->enunciado, temp_enunciado, sizeof(p->enunciado) - 1); 
    p->enunciado[sizeof(p->enunciado) - 1] = '\0'; 
    free(temp_enunciado); // Libera a memória alocada por leTextoDinamico
    converteMaiscula(p->enunciado); // Converte para maiúsculas

    // Recebe alternativas
    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        p->alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        char *temp_texto = leTextoDinamico(); // Lê o texto dinamicamente
        // Copia o texto lido para o array fixo na struct e garante terminação nula
        strncpy(p->alternativas[i].texto, temp_texto, sizeof(p->alternativas[i].texto) - 1); 
        p->alternativas[i].texto[sizeof(p->alternativas[i].texto) - 1] = '\0'; 
        free(temp_texto); // Libera a memória alocada por leTextoDinamico
        converteMaiscula(p->alternativas[i].texto);
        p->alternativas[i].removida = 0;
    }

    // Recebe a alternativa correta
    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &p->correta);
    p->correta = toupper(p->correta);

    // Recebe a dificuldade
    printf("Escolha a dificuldade da questão:\n");
    printf("1 - Muito Fácil\n2 - Fácil\n3 - Médio\n4 - Difícil\n5 - Muito Difícil\n");
    int nivel_input;
    scanf("%d", &nivel_input);
    // Valida o nível, caso seja inválido, define como 1
    if (nivel_input < 1 || nivel_input > 5) {
        nivel_input = 1;
    }
    p->nivel = (Dificuldade) nivel_input; // Converte para o enum Dificuldade
    limpaBuffer(); // Limpa o buffer após scanf

    p->ja_foi_usada = 0; // Inicializa o campo 'ja_foi_usada'
    (*total)++; // Incrementa o contador total de perguntas
    printf("Pergunta adicionada com sucesso!\n");

    // Salva as perguntas atualizadas no CSV
    salvaPerguntasNoCSV(*perguntas, "questoes.csv", *total); 
}

// Mostra uma pergunta completa (enunciado e alternativas)
void mostraPergunta(Pergunta *pergunta) {
    printf("%s\n", pergunta->enunciado);
    for (int i = 0; i < 4; i++) {
        printf("%c) %s\n", pergunta->alternativas[i].letra, pergunta->alternativas[i].texto);
    }
}

// Lista perguntas por nível de dificuldade
void listaPerguntas(Pergunta *perguntas, int total) {
    for (int nivel_loop = 1; nivel_loop <= 5; nivel_loop++) { // Loop pelos níveis de 1 a 5
        printf("\n       Nivel %d       \n", nivel_loop);
        int perguntas_no_nivel = 0;
        for (int i = 0; i < total; i++) {
            if ((int)perguntas[i].nivel == nivel_loop) { 
                printf("Indice %d: \n", i);
                mostraPergunta(&perguntas[i]);
                printf("\n");
                perguntas_no_nivel++;
            }
        }
        if (perguntas_no_nivel == 0) {
            printf("Nao ha perguntas para o Nivel %d.\n", nivel_loop);
        }
    }
}

// Pesquisa pergunta pelo enunciado
void pesquisaPergunta(Pergunta *perguntas, int total) {
    printf("Digite a palavra para pesquisar: \n");
    char *pesquisa = leTextoDinamico();
    converteMaiscula(pesquisa); // Converte a pesquisa para maiúsculas
    int encontrada = 0;

    for (int i = 0; i < total; i++) {
        // strstr busca a substring 'pesquisa' dentro de 'perguntas[i].enunciado'
        if (strstr(perguntas[i].enunciado, pesquisa)) {
            printf("Indice %d: \n", i);
            mostraPergunta(&perguntas[i]);
            encontrada = 1;
        }
    }
    if (!encontrada) {
        printf("Pergunta não encontrada!\n");
    }
    free(pesquisa); // Libera a memória da string de pesquisa
}

// Altera pergunta existente com base no índice
void alterarPergunta(Pergunta *perguntas, int total) {
    int indice;
    printf("Qual índice da pergunta deseja alterar? ");
    scanf("%d", &indice);
    limpaBuffer(); // Limpa o buffer após scanf

    if (indice < 0 || indice >= total) {
        printf("Índice inválido!\n");
        return;
    }

    printf("Digite o enunciado da nova questão:\n");
    char *temp_enunciado = leTextoDinamico();
    // Copia o novo enunciado para o array fixo
    strncpy(perguntas[indice].enunciado, temp_enunciado, sizeof(perguntas[indice].enunciado) - 1);
    perguntas[indice].enunciado[sizeof(perguntas[indice].enunciado) - 1] = '\0';
    free(temp_enunciado);
    converteMaiscula(perguntas[indice].enunciado);

    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        perguntas[indice].alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        char *temp_texto = leTextoDinamico();
        // Copia o novo texto da alternativa para o array fixo
        strncpy(perguntas[indice].alternativas[i].texto, temp_texto, sizeof(perguntas[indice].alternativas[i].texto) - 1);
        perguntas[indice].alternativas[i].texto[sizeof(perguntas[indice].alternativas[i].texto) - 1] = '\0';
        free(temp_texto);
        converteMaiscula(perguntas[indice].alternativas[i].texto);
    }

    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &perguntas[indice].correta);
    perguntas[indice].correta = toupper(perguntas[indice].correta);
    
    int nivel_input;
    printf("Digite a dificuldade da pergunta (1-5):\n");
    scanf("%d", &nivel_input);
    if (nivel_input < 1 || nivel_input > 5) {
        nivel_input = 1;
    }
    perguntas[indice].nivel = (Dificuldade) nivel_input;
    limpaBuffer();

    // Salva as perguntas atualizadas no CSV
    salvaPerguntasNoCSV(perguntas, "questoes.csv", total);
    printf("Pergunta %d alterada com sucesso!\n", indice);
}

// Exclui pergunta e reorganiza vetor
void excluirPergunta(Pergunta **perguntas, int *total) {
    int indice;
    printf("Qual índice da pergunta deseja excluir (0 a %d)? ", *total - 1);
    scanf("%d", &indice);
    limpaBuffer(); // Limpa o buffer após scanf

    if (indice < 0 || indice >= *total) {
        printf("Índice inválido!\n");
        return;
    }
    
    // Move as perguntas posteriores para cobrir o espaço da pergunta excluída
    for (int i = indice; i < *total - 1; i++) {
        (*perguntas)[i] = (*perguntas)[i + 1];
    }

    // Realoca o vetor para um tamanho menor
    Pergunta *temp = realloc(*perguntas, (*total - 1) * sizeof(Pergunta));
    
    // Se o novo total for 0, temp pode ser NULL, o que é um estado válido (vetor vazio)
    if (temp != NULL || (*total - 1) == 0) {
        *perguntas = temp;
        (*total)--; // Decrementa o contador total de perguntas
        printf("Pergunta excluída com sucesso!\n");
    } else {
        printf("Erro ao realocar vetor de perguntas após exclusão!\n");
    }

    // Salva as perguntas atualizadas no CSV
    salvaPerguntasNoCSV(*perguntas, "questoes.csv", *total);
}