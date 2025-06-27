// sources/telas_jogo.c - Implementações para as Telas do Jogo Raylib (com Dicas, Posicionamento e Sons)

#include "telas_jogo.h" // Inclui o cabeçalho das telas para ver os protótipos e variáveis globais
#include "ranking.h"    // Inclui o cabeçalho do ranking para usar as funções lerRanking, salvarRanking, etc.
#include "pergunta.h"
#include <stdio.h>      // Para printf, sprintf
#include <string.h>     // Para strlen, strcpy, strncpy, memset
#include <ctype.h>      // Para toupper
#include <stdlib.h>     // Para rand (embora GetRandomValue da Raylib seja preferível)
#include <time.h>       // Para srand e time

int ProximaPerguntaIndex(void) {
    // Itera a partir da pergunta atual + 1 para encontrar a próxima pergunta não usada.
    // Se chegar ao fim, pode reiniciar do começo ou indicar que não há mais perguntas.
    for (int i = g_current_question_idx + 1; i < g_total_perguntas; ++i) {
        if (g_perguntas[i].ja_foi_usada == 0) {
            return i;
        }
    }
    // Se não encontrar uma próxima pergunta não usada após o índice atual,
    // tenta do início do array. Isso pode levar a um loop se todas forem usadas.
    for (int i = 0; i < g_total_perguntas; ++i) {
        if (g_perguntas[i].ja_foi_usada == 0) {
            return i;
        }
    }
    return -1; // Retorna -1 se não houver perguntas disponíveis ou todas foram usadas
}


// --- Implementações das Funções Auxiliares de UI Gráfica ---
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor) {
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();
    
    // Verifica se o mouse está sobre o botão
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        buttonColor = Fade(buttonColor, 0.7f); // Escurece um pouco ao passar o mouse
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked = true;
            PlaySound(g_sound_menu_click); // Toca som de clique no menu
        }
    }
    
    DrawRectangleRec(bounds, buttonColor);
    // Para centralizar o texto usando a fonte padrão, que MeasureText usa
    DrawText(text, bounds.x + bounds.width/2 - MeasureText(text, 20)/2, bounds.y + bounds.height/2 - 10, 20, textColor);
    
    return clicked;
}

void ApplyFiftyFiftyHint(Pergunta *question) {
    // Coleta as letras das alternativas incorretas
    char incorrect_options[3]; // Max 3 opções incorretas (A, B, C, D - 1 correct)
    int incorrect_count = 0;
    for (int i = 0; i < 4; i++) {
        if (toupper(question->alternativas[i].letra) != toupper(question->correta)) {
            incorrect_options[incorrect_count++] = question->alternativas[i].letra;
        }
    }

    if (incorrect_count >= 2) {
        // Inicializa o gerador de números aleatórios se ainda não foi
        if (g_hint_fifty_fifty_used == 0) { // Inicializa o gerador apenas uma vez por sessão de jogo para não ter números aleatórios iguais.
            srand((unsigned int)time(NULL));
        }
        
        // Seleciona duas alternativas incorretas aleatoriamente para eliminar
        int idx1 = GetRandomValue(0, incorrect_count - 1);
        char eliminated1 = incorrect_options[idx1];

        int idx2;
        do {
            idx2 = GetRandomValue(0, incorrect_count - 1);
        } while (idx2 == idx1); // Garante que a segunda opção seja diferente da primeira
        char eliminated2 = incorrect_options[idx2];

        g_fifty_fifty_eliminated_chars[0] = eliminated1;
        g_fifty_fifty_eliminated_chars[1] = eliminated2;
        printf("50/50 ativado! Eliminadas: %c e %c\n", eliminated1, eliminated2);
    } else {
        printf("Nao foi possivel aplicar 50/50, menos de 2 alternativas incorretas.\n");
    }
}


void ResetGamePlayingState(void) {
    g_current_level = 1;
    g_correct_answers_in_row = 0;
    g_current_question_idx = -1; 
    g_game_play_state = PLAYING_QUESTION;
    g_answer_feedback_timer = 0;
    g_is_answer_correct = false;
    g_selected_answer_char = ' ';
    g_correct_answer_char = ' '; // Mantido como 'correta' conforme pergunta.h
    
    // RESETAR CONTADORES DE DICAS
    g_hint_exclude_used = 0;
    g_hint_skip_used = 0;
    g_hint_fifty_fifty_used = 0; // Reseta o contador
    g_fifty_fifty_active = false; // Desativa a dica 50/50
    g_fifty_fifty_eliminated_chars[0] = 0; // Limpa caracteres eliminados
    g_fifty_fifty_eliminated_chars[1] = 0;
    
    // Resetar estado da tela de fim de jogo para a próxima vez que for acessada
    g_game_ending_state = ENDING_SHOW_SCORE; 
    memset(g_player_name_input, 0, sizeof(g_player_name_input)); // Limpa o buffer do nome
    g_player_name_chars_count = 0;
    g_player_final_score = 0; // Resetar pontuação final
}

void LoadNextQuestion(void) {
    Pergunta *question = NULL;
    int levelToTry = g_current_level;

    // Tenta níveis do atual até o 5 (milhão)
    while (levelToTry <= 5) {
        question = sorteiaPorNivel(g_perguntas, g_total_perguntas, levelToTry);
        if (question != NULL) {
            // Atualiza o nível atual caso tenha subido
            g_current_level = levelToTry;
            break;
        }
        levelToTry++; // sobe nível e tenta de novo
    }

    if (question != NULL) {
        for (int i = 0; i < g_total_perguntas; ++i) {
            if (&g_perguntas[i] == question) {
                g_current_question_idx = i;
                break;
            }
        }
        g_game_play_state = PLAYING_QUESTION;
        g_answer_feedback_timer = 0;
        g_is_answer_correct = false;
        g_selected_answer_char = ' ';
        g_correct_answer_char = question->correta;
        g_fifty_fifty_active = false; // Reset 50/50 for new question
        g_fifty_fifty_eliminated_chars[0] = 0;
        g_fifty_fifty_eliminated_chars[1] = 0;
    } else {
        printf("Nenhuma pergunta encontrada para níveis %d a 5! Finalizando jogo.\n", g_current_level);
        SetGameScreen(GAME_ENDING); // Vai para a tela de fim do jogo/ranking
    }
}

// Função para mudar a tela do jogo
void SetGameScreen(GameScreen screen) {
    currentScreen = screen;
}

// --- Implementações das Funções de Update (Lógica de cada tela) ---
void UpdateLogoScreen(void) {
    framesCounter++;
    if (framesCounter > 180) SetGameScreen(GAME_TITLE); // Usar SetGameScreen
}

void UpdateTitleScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
        PlaySound(g_sound_menu_click); // Som ao ir para o menu
    }
}

void UpdateMainMenuScreen(void) {
    int screenWidth = GetScreenWidth();
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = GetScreenHeight() / 3; // ALINHADO COM DrawMainMenuScreen
    float padding = 20;

    // Botão JOGAR
    Rectangle playButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY, buttonWidth, buttonHeight };
    if (GuiButton(playButtonBounds, "JOGAR", MARVEL_BLUE, RAYWHITE)) { 
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para jogar! Insira algumas primeiro (CONSOLE).\n");
        } else {
            ResetGamePlayingState();
            LoadNextQuestion();
            SetGameScreen(GAME_PLAYING_GRAPHICAL); // Usar SetGameScreen
            StopMusicStream(g_music_background); // Para a música de fundo quando o jogo começa
        }
    }

    // Botão EDITAR PERGUNTAS
    Rectangle alterButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(alterButtonBounds, "EDITAR PERGUNTAS", MARVEL_BLUE, RAYWHITE)) {
        SetGameScreen(GAME_EDIT_MENU);
    }
    
    // Botão LISTAR PERGUNTAS
    Rectangle listButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(listButtonBounds, "LISTAR PERGUNTAS", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para listar (CONSOLE).\n");
        } else {
            printf("\n--- LISTANDO PERGUNTAS (VERIFIQUE O CONSOLE) ---\n");
           listaPerguntas(g_perguntas, g_total_perguntas); // Passando total_perguntas por referência
           printf("--- FIM DA LISTAGEM NO CONSOLE ---\n");
        }
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }
    
    // Botão PESQUISAR PERGUNTA
    Rectangle searchButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(searchButtonBounds, "PESQUISAR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para pesquisar (CONSOLE).\n");
        } else {
            printf("\n--- PESQUISANDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            pesquisaPergunta(g_perguntas, g_total_perguntas);
            printf("--- FIM DELLA RICERCA NEL CONSOLE ---\n");
        }
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }
    
    // NOVO Botão: Ranking
    Rectangle rankingButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(rankingButtonBounds, "RANKING", MARVEL_BLUE, RAYWHITE)) {
        SetGameScreen(GAME_RANKING); // Ir para a tela de ranking
    }
    
    // Botão SAIR (posicionado abaixo do novo botão Ranking)
    Rectangle exitButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 5 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(exitButtonBounds, "SAIR", MARVEL_RED, RAYWHITE)) {
        SetGameScreen(GAME_EXIT); // Usar SetGameScreen
    }
}

void UpdateEditMenuScreen(void){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = screenHeight / 4;
    float padding = 20;

    // Botão INSERIR PERGUNTA
    Rectangle insertButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + buttonHeight + padding, buttonWidth, buttonHeight };
    if (GuiButton(insertButtonBounds, "INSERIR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        printf("\n--- INSERINDO PERGUNTA (INTERAJA PELO CONSOLE) ---\n");
        recebePergunta(&g_perguntas, &g_total_perguntas);
        printf("--- FIM DA INSERCAO NO CONSOLE ---\n");
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }
    
    // Botão ALTERAR PERGUNTA
    Rectangle alterButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(alterButtonBounds, "ALTERAR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para alterar (CONSOLE).\n");
        } else {
            printf("\n--- ALTERANDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            alterarPergunta(g_perguntas, g_total_perguntas);
            printf("--- FIM DA ALTERACAO NO CONSOLE ---\n");
        }
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }
    
    // Botão EXCLUIR PERGUNTA
    Rectangle deleteButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(deleteButtonBounds, "EXCLUIR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para excluir (CONSOLE).\n");
        } else {
            printf("\n--- EXCLUINDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            excluirPergunta(&g_perguntas, &g_total_perguntas);
            printf("--- FIM DA EXCLUSAO NO CONSOLE ---\n");
        }
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }

    // Botão VOLTAR
    Rectangle backButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(backButtonBounds, "VOLTAR", MARVEL_RED, RAYWHITE)) {
        SetGameScreen(GAME_MAIN_MENU);
    }
}


void UpdateDisplayQuestionsScreen(void) {
    if (IsKeyPressed(KEY_SPACE)) {
        g_current_display_question_idx++;
        if (g_current_display_question_idx >= g_total_perguntas) {
            g_current_display_question_idx = 0;
            SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        SetGameScreen(GAME_MAIN_MENU); // Usar SetGameScreen
    }
}

void UpdatePlayingGraphicalScreen(void) {
    const int screenWidth = GetScreenWidth();

    if (g_current_question_idx == -1) {
        // Sem mais perguntas, vai para tela de fim de jogo
        g_player_final_score = g_correct_answers_in_row * 1000;
        g_game_ending_state = ENDING_SHOW_SCORE;
        SetGameScreen(GAME_ENDING);
        return;
    }

    Pergunta *current_question = &g_perguntas[g_current_question_idx];

    switch (g_game_play_state) {
        case PLAYING_QUESTION: {
            float hintButtonWidth = 100;
            float hintButtonHeight = 40;
            float hintPadding = 10;
            float hintX_Rightmost = screenWidth - hintButtonWidth - 20;
            float hintX_Middle = hintX_Rightmost - hintButtonWidth - hintPadding;
            float hintX_Leftmost = hintX_Middle - hintButtonWidth - hintPadding; 
            float hintStartY = 50;

            // Lógica para a dica 50/50
            Rectangle fiftyFiftyButtonBounds = { hintX_Leftmost, hintStartY, hintButtonWidth, hintButtonHeight };
            Color fiftyFiftyColor = (g_hint_fifty_fifty_used < 3) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(fiftyFiftyButtonBounds, "50/50", fiftyFiftyColor, RAYWHITE)) {
                if (g_hint_fifty_fifty_used < 3) {
                    ++g_hint_fifty_fifty_used; // Incrementa o contador de uso
                    g_fifty_fifty_active = true;
                    ApplyFiftyFiftyHint(current_question); // Chama a função para aplicar a dica
                    PlaySound(g_sound_snap); // Som de clique da dica
                }
            }

            // Lógica para o botão "Excluir Questão"
            Rectangle excludeButtonBounds = { hintX_Middle, hintStartY, hintButtonWidth, hintButtonHeight };
            Color excludeColor = (g_hint_exclude_used == 0) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(excludeButtonBounds, "EXCLUIR", excludeColor, RAYWHITE)) {
                if (g_hint_exclude_used == 0) {
                    g_hint_exclude_used = 1;
                    for (int i = 0; i < 4; i++) {
                        if (toupper(current_question->alternativas[i].letra) != toupper(current_question->correta) &&
                            current_question->alternativas[i].removida == 0) {
                            current_question->alternativas[i].removida = 1; // This 'removida' flag is not used for 50/50, only for "excluir" hint.
                            break;
                        }
                    }
                    PlaySound(g_sound_snap); // Som de clique da dica, mantido
                }
            }

            // Lógica para o botão "Pular Questão"
            Rectangle skipButtonBounds = { hintX_Rightmost, hintStartY, hintButtonWidth, hintButtonHeight };
            Color skipColor = (g_hint_skip_used < 2) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(skipButtonBounds, "PULAR", skipColor, RAYWHITE)) {
                if (g_hint_skip_used < 2) {
                    g_hint_skip_used++;
                    current_question->ja_foi_usada = 1;
                    g_current_question_idx = ProximaPerguntaIndex();
                    g_game_play_state = LEVEL_COMPLETE;
                    PlaySound(g_sound_snap); // Som de clique da dica, mantido
                }
            }

            // Detecta cliques nas alternativas e entradas de teclado
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                char selected = ' ';
                float optionY = 250;
                float optionHeight = 60;
                float optionPadding = 20;

                for (int i = 0; i < 4; i++) {
                    bool is_eliminated = false;
                    if (g_fifty_fifty_active) {
                        if (toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[0]) ||
                            toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[1])) {
                            is_eliminated = true;
                        }
                    }

                    if (!is_eliminated) { //Só verifica colisão se não foi eliminada pelo 50/50
                        Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
                        if (CheckCollisionPointRec(mousePoint, optionBounds)) {
                            selected = current_question->alternativas[i].letra;
                            break;
                        }
                    }
                }
                if (selected != ' ') {
                    g_selected_answer_char = selected;
                    g_is_answer_correct = (toupper(selected) == toupper(current_question->correta));
                    g_game_play_state = SHOWING_FEEDBACK; 
                    g_answer_feedback_timer = 0;
                    if (g_is_answer_correct) PlaySound(g_sound_correct);
                    else PlaySound(g_sound_wrong);
                }
            }
            // Teclas A, B, C, D (AJUSTADO PARA 50/50 e nome 'correta')
            if (IsKeyPressed(KEY_A) && !(g_fifty_fifty_active && (toupper('A') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('A') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'A'; g_is_answer_correct = (toupper('A') == toupper(current_question->correta)); g_game_play_state = SHOWING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_B) && !(g_fifty_fifty_active && (toupper('B') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('B') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'B'; g_is_answer_correct = (toupper('B') == toupper(current_question->correta)); g_game_play_state = SHOWING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_C) && !(g_fifty_fifty_active && (toupper('C') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('C') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'C'; g_is_answer_correct = (toupper('C') == toupper(current_question->correta)); g_game_play_state = SHOWING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_D) && !(g_fifty_fifty_active && (toupper('D') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('D') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'D'; g_is_answer_correct = (toupper('D') == toupper(current_question->correta)); g_game_play_state = SHOWING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }

        } break;
        case SHOWING_FEEDBACK: { 
            g_answer_feedback_timer++;
            if (g_answer_feedback_timer >= 60 * 2) { 
                if (g_is_answer_correct) {
                    g_correct_answers_in_row++;
                    if (g_correct_answers_in_row == 2 || g_correct_answers_in_row == 4 || g_correct_answers_in_row == 8 || g_correct_answers_in_row == 12 || g_correct_answers_in_row == 15) {
                        g_current_level++;
                        printf("Progrediu para o nível %d!\n", g_current_level);
                    }
                    if (g_correct_answers_in_row >= 15) {
                        g_player_final_score = 1000000; // Ganhou o milhão!
                        SetGameScreen(GAME_WIN); // Usar SetGameScreen
                        PlaySound(g_sound_win); // Som de vitória
                    } else {
                        g_game_play_state = LEVEL_COMPLETE; // Vai para o próximo nível
                    }
                } else {
                    // CORREÇÃO PARA A PONTUAÇÃO DE PERDA:
                    g_player_final_score = g_correct_answers_in_row * 1000; 
                    SetGameScreen(GAME_LOSE); // Usar SetGameScreen
                    PlaySound(g_sound_lose); // Som de derrota
                }
            }
        } break;
        case LEVEL_COMPLETE: { 
            LoadNextQuestion();
        } break;
        default: break; 
    }
}

void UpdateInsertQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) SetGameScreen(GAME_MAIN_MENU); 
}
void UpdateListQuestionsScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) SetGameScreen(GAME_MAIN_MENU); 
}
void UpdateSearchQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) SetGameScreen(GAME_MAIN_MENU); 
}
void UpdateAlterQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) SetGameScreen(GAME_MAIN_MENU); 
}
void UpdateDeleteQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) SetGameScreen(GAME_MAIN_MENU); 
}

// --- ATUALIZAÇÃO DA TELA DE FIM DE JOGO (GAME_ENDING) ---
void UpdateEndingScreen(void) {
    framesCounter++; // Usa para o cursor piscando

    const int screenHeight = GetScreenHeight(); // screenWidth removido
    switch (g_game_ending_state) {
        case ENDING_SHOW_SCORE: {
            if (IsKeyPressed(KEY_ENTER)) {
                // Ao pressionar ENTER, verifica se a pontuação é digna do ranking
                // O score inicial no ranking é -1. Se for maior que -1 OU maior que o último score
                if (g_player_final_score > -1 && (g_ranking_board != NULL && (g_ranking_board[LEADER_SIZE - 1].score == -1 || g_player_final_score > g_ranking_board[LEADER_SIZE - 1].score))) {
                    g_game_ending_state = ENDING_INPUT_NAME; // Vai para a entrada de nome
                    // Resetar o buffer de nome
                    memset(g_player_name_input, 0, sizeof(g_player_name_input)); 
                    g_player_name_chars_count = 0;
                } else {
                    // Pontuação não é alta o suficiente, pula a entrada de nome e vai direto para o ranking ou menu
                    g_game_ending_state = ENDING_SHOW_RANKING; // Ainda mostra o ranking, mas sem o nome do jogador atual
                }
            }
        } break;
        case ENDING_INPUT_NAME: {
            // Lógica para coletar o nome do jogador
            int key = GetCharPressed();
            while (key > 0) {
                // Aceita caracteres imprimíveis e não excede o limite (MAX_LENGTH - 1 para o null terminator)
                if ((key >= 32) && (key <= 125) && (g_player_name_chars_count < MAX_LENGTH - 1)) {
                    g_player_name_input[g_player_name_chars_count] = (char)key;
                    g_player_name_chars_count++;
                    g_player_name_input[g_player_name_chars_count] = '\0'; // Garante terminação nula
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (g_player_name_chars_count > 0) {
                    g_player_name_chars_count--;
                    g_player_name_input[g_player_name_chars_count] = '\0'; // Remove o último caractere
                }
            }

            // Se ENTER for pressionado e houver pelo menos um caractere
            if (IsKeyPressed(KEY_ENTER) && g_player_name_chars_count > 0) {
                // Adiciona ao ranking e salva
                adicionarRanking(g_ranking_board, g_player_name_input, g_player_final_score);
                salvarRanking(g_ranking_board);

                g_game_ending_state = ENDING_SHOW_RANKING; // Exibe o ranking
            }
        } break;
        case ENDING_SHOW_RANKING: {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                SetGameScreen(GAME_MAIN_MENU); // Volta ao menu principal
                g_game_ending_state = ENDING_SHOW_SCORE; // Resetar para o próximo jogo
                PlayMusicStream(g_music_background);
            }
        } break;
    }
}

// Quando você ganha, vai para a tela de fim de jogo para registrar o score
void UpdateWinScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) SetGameScreen(GAME_ENDING);
}

// Quando você perde, vai para a tela de fim de jogo para registrar o score
void UpdateLoseScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) SetGameScreen(GAME_ENDING);
}

// --- NOVA TELA: RANKING (para ver a qualquer momento) ---
void UpdateRankingScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        SetGameScreen(GAME_MAIN_MENU); // Volta ao menu
    }
}


// --- Implementações das Funções de Draw (Desenho de cada tela) ---
void DrawLogoScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "MARVEL - O DESAFIO", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "MARVEL - O DESAFIO", 40, 0).x/2, screenHeight/2 - 50}, 
               40, 0, MARVEL_RED);
    DrawTextEx(g_marvel_font, "Preparando o Universo...", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "Preparando o Universo...", 20, 0).x/2, screenHeight/2 + 10}, 
               20, 0, MARVEL_LIGHTGRAY);
}

void DrawTitleScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "SHOW DO MILHAO", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "SHOW DO MILHAO", 80, 0).x/2, screenHeight/4}, 
               80, 0, MARVEL_GOLD);
    DrawTextEx(g_marvel_font, "MARVEL", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "MARVEL", 60, 0).x/2, screenHeight/4 + 80}, 
               60, 0, MARVEL_RED);
    DrawText("Pressione ENTER para comecar", screenWidth/2 - MeasureText("Pressione ENTER para comecar", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}

void DrawMainMenuScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("MENU PRINCIPAL", screenWidth/2 - MeasureText("MENU PRINCIPAL", 40)/2, screenHeight/8, 40, MARVEL_GOLD);
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = screenHeight / 3; //Agora alinhado com UpdateMainMenuScreen
    float padding = 20;

    // Desenha todos os botões (sem a lógica de clique, que está no Update)
    // A ordem e o texto devem espelhar a lógica em UpdateMainMenuScreen
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY, buttonWidth, buttonHeight }, "JOGAR", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + (buttonHeight + padding), buttonWidth, buttonHeight }, "EDITAR PERGUNTAS", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight }, "LISTAR PERGUNTAS", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight }, "PESQUISAR PERGUNTA", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight }, "RANKING", MARVEL_BLUE, RAYWHITE); // NOVO BOTÃO
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 5 * (buttonHeight + padding), buttonWidth, buttonHeight }, "SAIR", MARVEL_RED, RAYWHITE);
}

void DrawEditMenuScreen(void){
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("EDIT MENU", screenWidth/2 - MeasureText("EDIT MENU", 40)/2, screenHeight/8, 40, MARVEL_GOLD);
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = screenHeight / 4;
    float padding = 20;
    
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + buttonHeight + padding, buttonWidth, buttonHeight }, "INSERIR PERGUNTA", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight }, "ALTERAR PERGUNTA", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight }, "EXCLUIR PERGUNTA", MARVEL_BLUE, RAYWHITE);
    GuiButton((Rectangle){ (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight }, "VOLTAR", MARVEL_RED, RAYWHITE);
}

void DrawDisplayQuestionsScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    if (g_total_perguntas > 0 && g_current_display_question_idx < g_total_perguntas) {
        Pergunta *currentQuestion = &g_perguntas[g_current_display_question_idx];

        DrawText(TextFormat("PERGUNTA %d (NIVEL: %d)", g_current_display_question_idx + 1, currentQuestion->nivel), 50, 50, 25, MARVEL_GOLD);
        DrawTextEx(g_marvel_font, currentQuestion->enunciado, 
                                (Vector2){50, 100}, 20, 2, RAYWHITE);

        float altStartY = 350;
        for (int i = 0; i < 4; i++) {
            char altText[256];
            sprintf(altText, "%c) %s", currentQuestion->alternativas[i].letra, currentQuestion->alternativas[i].texto);
            
            Color altColor = RAYWHITE;
            if (toupper(currentQuestion->alternativas[i].letra) == toupper(currentQuestion->correta)) { // ***CORRIGIDO: 'correta'***
                altColor = COLOR_CORRETO;
            }
            DrawText(altText, 50, altStartY + (i * 30), 20, altColor);
        }

        DrawText("Pressione ESPACO para a proxima pergunta, ESC para voltar ao menu.", 
                                 50, screenHeight - 40, 15, MARVEL_LIGHTGRAY);

    } else {
        DrawText("Nenhuma pergunta para exibir. Volte ao menu.", screenWidth/2 - MeasureText("Nenhuma pergunta para exibir. Volho ao menu.", 20)/2, screenHeight/2, 20, RAYWHITE);
        DrawText("Pressione ESC para voltar ao menu.", screenWidth/2 - MeasureText("Pressione ESC para voltar ao menu.", 15)/2, screenHeight/2 + 30, 15, MARVEL_LIGHTGRAY);
    }
}

void DrawPlayingGraphicalScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    if (g_current_question_idx != -1) {
        Pergunta *current_question = &g_perguntas[g_current_question_idx];

        // Desenhar Nível e Respostas Corretas
        DrawText(TextFormat("NIVEL %d", g_current_level), 50, 20, 30, MARVEL_GOLD);
        DrawText(TextFormat("RESPOSTAS CORRETAS: %d", g_correct_answers_in_row), screenWidth - 300, 20, 20, MARVEL_LIGHTGRAY); 
        
        // Desenhar Enunciado da Pergunta com fonte personalizada
        DrawTextEx(g_marvel_font, current_question->enunciado,
                                (Vector2){50, 100}, 20, 2, RAYWHITE);

        // Desenhar Alternativas com fonte personalizada
        float optionY = 250;
        float optionHeight = 60;
        float optionPadding = 20;

        for (int i = 0; i < 4; i++) {
            Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
            Color optionColor = MARVEL_BLUE;

            bool is_eliminated_by_fifty_fifty = false;
            if (g_fifty_fifty_active) {
                if (toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[0]) ||
                    toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[1])) {
                    is_eliminated_by_fifty_fifty = true;
                }
            }

            if (is_eliminated_by_fifty_fifty) {
                optionColor = DARKGRAY; // Mostra em cinza as opções eliminadas
                DrawRectangleRec(optionBounds, optionColor);
                char eliminatedText[256];
                snprintf(eliminatedText, sizeof(eliminatedText), "%c) ELIMINADA", current_question->alternativas[i].letra);
                DrawTextEx(g_marvel_font, eliminatedText,
                                         (Vector2){optionBounds.x + 15, optionBounds.y + optionHeight / 2 - 10},
                                         20, 2, RAYWHITE);
            } else {
                if (g_game_play_state == SHOWING_FEEDBACK) { 
                    if (toupper(current_question->alternativas[i].letra) == toupper(g_selected_answer_char)) {
                        optionColor = g_is_answer_correct ? COLOR_CORRETO : COLOR_ERRADO;
                    } else if (toupper(current_question->alternativas[i].letra) == toupper(current_question->correta)) {
                        optionColor = COLOR_CORRETO;
                    }
                }
                DrawRectangleRec(optionBounds, optionColor);

                // Texto da alternativa com fonte personalizada
                char textoAlternativa[256];
                snprintf(textoAlternativa, sizeof(textoAlternativa), "%c) %s", current_question->alternativas[i].letra, current_question->alternativas[i].texto);

                DrawTextEx(g_marvel_font, textoAlternativa,
                                         (Vector2){optionBounds.x + 15, optionBounds.y + optionHeight / 2 - 10},
                                         20, 2, RAYWHITE);
            }
        }

        // Mensagem de feedback (CORRETO/ERRADO)
        if (g_game_play_state == SHOWING_FEEDBACK) { 
            if (g_is_answer_correct) {
                DrawTextEx(g_marvel_font, "CORRETO!", 
                                         (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "CORRETO!", 50, 2).x / 2, screenHeight - 100}, 
                                         50, 2, COLOR_CORRETO);
            } else {
                DrawTextEx(g_marvel_font, "ERRADO!", 
                                         (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "ERRADO!", 50, 2).x / 2, screenHeight - 100}, 
                                         50, 2, COLOR_ERRADO);
            }
        }

        // Desenho dos botões de dica
        float hintButtonWidth = 100;
        float hintButtonHeight = 40;
        float hintPadding = 10;
        float hintX_Rightmost = screenWidth - hintButtonWidth - 20;
        float hintX_Middle = hintX_Rightmost - hintButtonWidth - hintPadding;
        float hintX_Leftmost = hintX_Middle - hintButtonWidth - hintPadding; 
        float hintStartY = 50;

        // Botão 50/50
        Rectangle fiftyFiftyButtonBounds = { hintX_Leftmost, hintStartY, hintButtonWidth, hintButtonHeight }; 
        Color fiftyFiftyColor = (g_hint_fifty_fifty_used < 3) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE; 
        GuiButton(fiftyFiftyButtonBounds, "50/50", fiftyFiftyColor, RAYWHITE);

        // Botão "Excluir"
        Rectangle excludeButtonBounds = { hintX_Middle, hintStartY, hintButtonWidth, hintButtonHeight };
        Color excludeColor = (g_hint_exclude_used == 0) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
        GuiButton(excludeButtonBounds, "EXCLUIR", excludeColor, RAYWHITE);

        // Botão "Pular"
        Rectangle skipButtonBounds = { hintX_Rightmost, hintStartY, hintButtonWidth, hintButtonHeight };
        Color skipColor = (g_hint_skip_used < 2) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
        GuiButton(skipButtonBounds, "PULAR", skipColor, RAYWHITE);


    } else {
        // Nenhuma pergunta válida
        DrawText("Nenhuma pergunta disponivel para jogar.", screenWidth/2 - MeasureText("Nenhuma pergunta disponivel para jogar.", 20)/2, screenHeight/2, 20, RAYWHITE);
        DrawText("Pressione ESC para voltar ao menu", screenWidth/2 - MeasureText("Pressione ESC para voltar ao menu.", 15)/2, screenHeight/2 + 30, 15, MARVEL_LIGHTGRAY);
    }
}


void DrawInsertQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawListQuestionsScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawSearchQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawAlterQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawDeleteQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}

void DrawEndingScreen(void) {
    framesCounter++; // Usa para o cursor piscando

    const int screenHeight = GetScreenHeight(); // screenWidth removido
    switch (g_game_ending_state) {
        case ENDING_SHOW_SCORE: {
            DrawText("FIM DE JOGO!", GetScreenWidth() / 2 - MeasureText("FIM DE JOGO!", 60) / 2, 100, 60, RAYWHITE);
            DrawText(TextFormat("Sua Pontuacao: %d", g_player_final_score), GetScreenWidth() / 2 - MeasureText(TextFormat("Sua Pontuacao: %d", g_player_final_score), 40) / 2, 200, 40, GOLD);
            DrawText("Pressione ENTER para continuar...", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para continuar...", 20) / 2, screenHeight - 50, 20, GRAY);
        } break;
        case ENDING_INPUT_NAME: {
            DrawText("Parabens! Digite seu nome para o Ranking:", GetScreenWidth() / 2 - MeasureText("Parabens! Digite seu nome para o Ranking:", 30) / 2, 100, 30, RAYWHITE);
            
            // Desenha a caixa de entrada de texto
            Rectangle textBox = { GetScreenWidth() / 2 - 150, 200, 300, 40 };
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawRectangleLinesEx(textBox, 2, MARVEL_GOLD); // Borda para a caixa de texto
            DrawText(g_player_name_input, textBox.x + 10, textBox.y + 10, 20, BLACK);
            
            // Desenha o cursor piscando
            if (((framesCounter / 30) % 2) == 0) { // Pisca a cada 0.5 segundos (30 frames)
                DrawText("_", textBox.x + 10 + MeasureText(g_player_name_input, 20), textBox.y + 10, 20, BLACK);
            }

            DrawText(TextFormat("Caracteres: %d/%d", g_player_name_chars_count, MAX_LENGTH - 1), 
                                 GetScreenWidth() / 2 - 150, 250, 15, GRAY);
            DrawText("Pressione ENTER para confirmar.", 
                                 GetScreenWidth() / 2 - MeasureText("Pressione ENTER para confirmar.", 20) / 2, screenHeight - 50, 20, GRAY);

        } break;
        case ENDING_SHOW_RANKING: {
            // Desenha o título do ranking
            DrawText("TOP 10 VINGADORES!", GetScreenWidth() / 2 - MeasureText("TOP 10 VINGADORES!", 40) / 2, 50, 40, MARVEL_GOLD);
            
            int startY = 120;
            // Percorre o ranking global e desenha cada entrada
            for (int i = 0; i < LEADER_SIZE; i++) {
                if (g_ranking_board != NULL && g_ranking_board[i].score != -1) { // Só desenha entradas válidas, verifica se g_ranking_board não é nulo
                    DrawText(TextFormat("%d. %s - %d pontos", i + 1, g_ranking_board[i].name, g_ranking_board[i].score), 
                                         GetScreenWidth() / 2 - 200, startY + i * 30, 25, RAYWHITE);
                } else {
                    DrawText(TextFormat("%d. --- Vazio ---", i + 1), 
                                         GetScreenWidth() / 2 - 200, startY + i * 30, 25, GRAY);
                }
            }
            DrawText("Pressione ENTER para voltar ao Menu Principal.", 
                                 GetScreenWidth() / 2 - MeasureText("Pressione ENTER para voltar ao Menu Principal.", 20) / 2, screenHeight - 50, 20, GRAY);

        } break;
    }
}

void DrawWinScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "VOCE GANHOU O JOGO DO MILHAO MARVEL!", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "VOCE GANHOU O JOGO DO MILHAO MARVEL!", 40, 0).x/2, screenHeight/3}, 
               40, 0, MARVEL_GOLD);
    DrawTextEx(g_marvel_font, "Parabens, Vingador!", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "Parabens, Vingador!", 30, 0).x/2, screenHeight/3 + 60}, 
               30, 0, MARVEL_LIGHTGRAY);
    DrawText("Pressione ENTER para continuar...", screenWidth/2 - MeasureText("Pressione ENTER para continuar...", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}

void DrawLoseScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "VOCE PERDEU! Tente novamente.", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "VOCE PERDEU! Tente novamente.", 40, 0).x/2, screenHeight/3}, 
               40, 0, MARVEL_RED);
    DrawText("A resposta correta era: ", screenWidth/2 - MeasureText("A resposta correta era: ", 25)/2, screenHeight/3 + 60, 25, MARVEL_LIGHTGRAY);
    DrawText(TextFormat("Alternativa %c", toupper(g_correct_answer_char)), screenWidth/2 - MeasureText(TextFormat("Alternativa %c", toupper(g_correct_answer_char)), 40)/2, screenHeight/3 + 100, 40, MARVEL_GOLD);
    DrawText("Pressione ENTER para continuar...", screenWidth/2 - MeasureText("Pressione ENTER para continuar...", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}

// --- NOVA TELA: RANKING (para ver a qualquer momento) ---
void DrawRankingScreen(void) {
    const int screenHeight = GetScreenHeight(); // screenWidth removido

    DrawText("TOP 10 VINGADORES!", GetScreenWidth() / 2 - MeasureText("TOP 10 VINGADORES!", 40) / 2, 50, 40, MARVEL_GOLD);
    
    int startY = 120;
    // Percorre o ranking global e desenha cada entrada
    for (int i = 0; i < LEADER_SIZE; i++) {
        if (g_ranking_board != NULL && g_ranking_board[i].score != -1) { // Só desenha entradas válidas, verifica se g_ranking_board não é nulo
            DrawText(TextFormat("%d. %s - %d pontos", i + 1, g_ranking_board[i].name, g_ranking_board[i].score), 
                                 GetScreenWidth() / 2 - 200, startY + i * 30, 25, RAYWHITE);
        } else {
            DrawText(TextFormat("%d. --- Vazio ---", i + 1), 
                                 GetScreenWidth() / 2 - 200, startY + i * 30, 25, GRAY);
        }
    }
    DrawText("Pressione ENTER ou ESC para voltar ao Menu Principal.", 
             GetScreenWidth() / 2 - MeasureText("Pressione ENTER ou ESC para voltar ao Menu Principal.", 20) / 2, screenHeight - 50, 20, GRAY);
}