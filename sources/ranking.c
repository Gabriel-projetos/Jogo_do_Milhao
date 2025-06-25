#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ranking.h" // Inclui o cabeçalho correto

//------------------------------------------------------------------------------------------
// Métodos do Ranking
//------------------------------------------------------------------------------------------

// Caminho do arquivo de ranking. Ajuste se o seu jogo estiver em outro diretório.
char nome_arquivo[30] = "data/ranking.dat"; 

// Função para ler o ranking do arquivo
Ranking* lerRanking(){
    // Aloca memória para o array de Rankings
    Ranking *ranking = calloc(LEADER_SIZE, sizeof(Ranking));
    if (ranking == NULL) {
        perror("Falha ao alocar memória para o Ranking. O jogo pode não funcionar corretamente.");
        // Em um jogo, você pode tentar continuar com um ranking vazio
        // ou fechar o programa dependendo da severidade.
        // Aqui, optamos por sair, mas em um jogo Raylib, pode ser melhor retornar NULL e tratar.
        exit(EXIT_FAILURE); 
    }

    FILE* arquivo = fopen(nome_arquivo, "rb"); // Abre o arquivo em modo binário de leitura
    
    if (!arquivo){
        // Se o arquivo não existe, inicializa o ranking com valores vazios/inválidos
        printf("Ranking não encontrado. Criando um vazio...\n");
        for (size_t i = 0; i < LEADER_SIZE; i++){
            ranking[i].name[0] = '\0'; // Nome vazio
            ranking[i].score = -1;     // Pontuação inválida
        }
    } else {
        // Se o arquivo existe, lê os dados para a memória
        fread(ranking, sizeof(Ranking), LEADER_SIZE, arquivo);
        fclose(arquivo); // Fecha o arquivo após a leitura
    }
    return ranking; // Retorna o ponteiro para o array de rankings
}

// Função para salvar o ranking no arquivo
void salvarRanking(Ranking *ranking){

    FILE* arquivo = fopen(nome_arquivo, "wb"); // Abre o arquivo em modo binário de escrita
    
    if (arquivo == NULL) {
        perror("Erro ao salvar o Ranking. Verifique permissões de pasta ou caminho.");
    } else {
        // Escreve o array de Rankings para o arquivo
        fwrite(ranking, sizeof(Ranking), LEADER_SIZE, arquivo);
        fclose(arquivo); // Fecha o arquivo após a escrita
    }
    return;
}

// Função para adicionar uma nova entrada ao ranking, mantendo a ordem (maior pontuação no topo)
int adicionarRanking(Ranking* ranking, char* nome, int pontuacao){

    // Cria uma nova entrada de ranking com os dados fornecidos
    Ranking novoRanking;
    strncpy(novoRanking.name, nome, MAX_LENGTH - 1); // Copia o nome, garantindo que não excede MAX_LENGTH
    novoRanking.name[MAX_LENGTH - 1] = '\0';         // Garante terminação nula
    novoRanking.score = pontuacao;

    // Condição para não adicionar se a pontuação for muito baixa ou inválida
    // Se a nova pontuação for menor ou igual à última pontuação do ranking
    // E a última posição já tiver uma pontuação válida (não -1), então não entra no ranking.
    if (pontuacao == -1 || (ranking[LEADER_SIZE - 1].score != -1 && pontuacao <= ranking[LEADER_SIZE - 1].score)) {
        return -1; // Retorna -1 para indicar que não foi adicionado
    }

    int i;
    // Loop de trás para frente para encontrar a posição correta da nova pontuação
    // e "empurrar" as pontuações menores para baixo no ranking.
    for (i = LEADER_SIZE - 1; i > 0; i--) {
        // Se a pontuação na posição anterior for MENOR que a nova pontuação,
        // move a entrada da posição anterior para a posição atual (empurra para baixo).
        if (ranking[i - 1].score < novoRanking.score) {
            ranking[i] = ranking[i - 1];
        } else {
            // Se a pontuação na posição anterior for MAIOR ou IGUAL à nova pontuação,
            // encontramos o lugar certo: a nova entrada vai na posição 'i'.
            break; 
        }
    }

    // Insere a nova entrada na posição encontrada (ou na posição 0 se for a maior)
    ranking[i] = novoRanking;

    return i; // Retorna o índice onde a nova entrada foi adicionada
}

// Função para imprimir o ranking (para console, útil para depuração)
void imprimirRanking(Ranking *ranking){

    printf("\n|   # | Nome                   | Pontos |\n");
    printf("|-----|------------------------|--------|\n"); // Separador para melhor visualização
    for (int i = 0; i < LEADER_SIZE; i++){
        // Só imprime se a entrada tiver uma pontuação válida
        if (ranking[i].score != -1) {
            printf("| %2d  | %-20s | %6d |\n", i + 1, ranking[i].name, ranking[i].score);
        } else {
            printf("| %2d  | %-20s | %6s |\n", i + 1, "--- Vazio ---", "----"); // Exibe como vazio
        }
    }
    printf("|-----|------------------------|--------|\n");
    return;
}

// Função para retornar a menor pontuação atual no ranking (a última posição)
int menorPontuacao(Ranking* ranking){
    return ranking[LEADER_SIZE - 1].score;
}

/*
int main(){
    printf("--- Testando Sistema de Ranking ---\n");

    // 1. Tenta ler o ranking existente ou cria um novo
    Ranking* ranking = lerRanking();

    printf("\nRanking Inicial:\n");
    imprimirRanking(ranking);

    // 2. Adiciona algumas pontuações para teste
    printf("\nAdicionando pontuacoes:\n");
    adicionarRanking(ranking, "Muriel", 50);
    imprimirRanking(ranking); // Para ver o ranking a cada adição

    adicionarRanking(ranking, "Mantova", 70);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Luizao", 60);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Marcelo", 90);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Adalberto", 30);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Maurichan", 50); // Deve entrar acima de Muriel se 50 for maior
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Fabiao", 70);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Wendel", 60);
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Wagner", 90); // Deve substituir o menor, se for maior
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Andre do Mato", 30); // Se 30 for muito baixo, não deve entrar
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Super High", 1000); // Pontuação alta
    imprimirRanking(ranking);

    adicionarRanking(ranking, "Low Score", 10); // Pontuação baixa
    imprimirRanking(ranking);

    // 3. Salva o ranking atual
    printf("\nSalvando Ranking...\n");
    salvarRanking(ranking);

    // 4. Libera a memória alocada
    free(ranking);
    ranking = NULL; // Boa prática para evitar 'dangling pointer'

    // 5. Opcional: recarrega para verificar se salvou corretamente
    printf("\nRecarregando Ranking para verificar...\n");
    ranking = lerRanking();
    imprimirRanking(ranking);
    free(ranking); // Libera novamente
    ranking = NULL;

    printf("\n--- Teste de Ranking Concluido ---\n");
    return 0;
}
*/
