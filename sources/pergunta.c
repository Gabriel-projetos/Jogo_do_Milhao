#include "../headers/pergunta.h"
#include "../headers/leitor_csv.h"

// Recebe nova pergunta, realocando o vetor de Pergunta*
void recebePergunta(Pergunta **perguntas, int *total) {
    Pergunta *nova = realloc(*perguntas, (*total + 1) * sizeof(Pergunta));
    if (!nova) {
        printf("Erro ao alocar memória para nova pergunta!\n");
        return ;
    }

    *perguntas = nova;

    Pergunta *p = &((*perguntas)[*total]);


    // Recebe o enunciado
    printf("Digite o enunciado da questão:\n");
    limpaBuffer();
    p->enunciado = leTextoDinamico();
    converteMaiscula(p->enunciado);

    // Recebe alternativas
    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        p->alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        p->alternativas[i].texto = leTextoDinamico();
        converteMaiscula(p->alternativas[i].texto);
    }

    // Recebe correta
    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &p->correta);
    p->correta = toupper(p->correta);

    // Recebe dificuldade
    printf("Escolha a dificuldade da questão:\n");
    printf("1 - Muito Fácil\n2 - Fácil\n3 - Médio\n4 - Difícil\n5 - Muito Difícil\n");
    int nivel;
    scanf("%d", &nivel);
    // Realoca o vetor de perguntas para guardar o novo ponteiro
    if (nivel < 1 || nivel > 5) nivel = 1;
    p->nivel = (Dificuldade) nivel;
    limpaBuffer();

    (*total)++;
    printf("Pergunta adicionada com sucesso!\n");
}


// Mostra uma pergunta completa
void mostraPergunta(Pergunta *pergunta) {
    printf("%s\n", pergunta->enunciado);
    for (int i = 0; i < 4; i++) {
        printf("%c) %s\n", pergunta->alternativas[i].letra, pergunta->alternativas[i].texto);
    }
}

// Lista perguntas por nível
void listaPerguntas(Pergunta *perguntas, int total) {
    for (int nivel = 1; nivel <= 5; nivel++) {
        printf("\n      Nivel %d      \n", nivel++);
        for (int i = 0; i < total; i++) {
            if ((int)perguntas[i].nivel == nivel) {
                printf("Indice %d: \n", i);
                mostraPergunta(&perguntas[i]);
                printf("\n");
            }
        }
    }
}

// Pesquisa pergunta pelo enunciado
void pesquisaPergunta(Pergunta *perguntas, int total) {
    printf("Digite a palavra para pesquisar: \n");
    char *pesquisa = leTextoDinamico();
    converteMaiscula(pesquisa);
    int encontrada = 0;

    for (int i = 0; i < total; i++) {
        if (strstr(perguntas[i].enunciado, pesquisa)) {
            printf("Indice  %d: \n", i);
            mostraPergunta(&perguntas[i]);
            encontrada = 1;
        }
    }
    if (!encontrada) printf("Pergunta não encontrada!\n");
    free(pesquisa);
}

// Altera pergunta existente
void alterarPergunta(Pergunta *perguntas, int total) {
    int indice;
    printf("Qual índice da pergunta deseja alterar? ");
    scanf("%d", &indice);
    limpaBuffer();

    if (indice < 0 || indice >= total) {
        printf("Índice inválido!\n");
        return;
    }

    // Libera memória da pergunta atual
    free(perguntas[indice].enunciado);
    for (int i = 0; i < 4; i++) free(perguntas[indice].alternativas[i].texto);

    // Recebe nova pergunta para substituir
    printf("Digite o enunciado da nova questão:\n");
    perguntas[indice].enunciado = leTextoDinamico();
    converteMaiscula(perguntas[indice].enunciado);

    char letras[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        perguntas[indice].alternativas[i].letra = letras[i];
        printf("Digite a alternativa %c:\n", letras[i]);
        perguntas[indice].alternativas[i].texto = leTextoDinamico();
        converteMaiscula(perguntas[indice].alternativas[i].texto);
    }

    printf("Digite qual é a alternativa correta (A/B/C/D):\n");
    scanf(" %c", &perguntas[indice].correta);
    perguntas[indice].correta = toupper(perguntas[indice].correta);
    
    int nivel;
    printf("Digite a dificuldade da pergunta (1-5):\n");
    scanf("%d", &nivel);
    if (nivel < 1 || nivel > 5) nivel = 1;
    perguntas[indice].nivel = (Dificuldade) nivel;
    limpaBuffer();

    printf("Pergunta %d alterada com sucesso!\n", indice);
}

// Exclui pergunta e reorganiza vetor
void excluirPergunta(Pergunta **perguntas, int *total) {
    int indice;
    printf("Qual índice da pergunta deseja excluir (0 a %d)? ", *total - 1);
    scanf("%d", &indice);
    limpaBuffer();

    if (indice < 0 || indice >= *total) {
        printf("Índice inválido!\n");
        return;
    }

    free((*perguntas)[indice].enunciado);
    for (int i = 0; i < 4; i++) free((*perguntas)[indice].alternativas[i].texto);

    for (int i = indice; i < *total - 1; i++) {
        (*perguntas)[i] = (*perguntas)[i + 1];
    }

    Pergunta *temp = realloc(*perguntas, (*total - 1) * sizeof(Pergunta));
    if (temp || *total - 1 == 0) {
        *perguntas = temp;
        (*total)--;
        printf("Pergunta excluída com sucesso!\n");
    } else {
        printf("Erro ao realocar vetor de perguntas!\n");
    }
}

//Salva a pergunta no CSV para não perder os dados
void salvaPerguntaNoCSV(const char *questoes, Pergunta *pergunta){
    FILE *file = fopen(questoes, "a");
    if(file == NULL){
        printf("Erro ao abrir o arquivo %s\n", questoes);
    }//if

    fprintf(file, "%s;", pergunta->enunciado);
    for(int i = 0; i < 4; i++){
        fprintf(file, "%s;", pergunta->alternativas[i].texto);
    }//for
    
    fprintf(file, "%c;%d\n", pergunta->correta, pergunta->nivel);
    fclose(file);
}//salvaPerguntaNoCSV
