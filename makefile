# Nome do executável final
EXEC = jogo_do_milhao

# Compilador usado
CC = gcc

# Diretórios
SRC_DIR = Sourcers
INC_DIR = Headers

# Arquivos fonte com nome com acento
SRCS = main.c $(SRC_DIR)/pergunta.c $(SRC_DIR)/funções_padrão.c

# Flags do compilador (inclui o diretório de headers)
CFLAGS = -I$(INC_DIR) -Wall -Wextra

# Regra padrão para compilar tudo
all: $(EXEC)

# Como compilar o executável
$(EXEC): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $(EXEC)

# Limpa os arquivos executáveis (Windows)
clean:
	del /f /q $(EXEC).exe 2>nul || rm -f $(EXEC)

# Executa o programa
run: $(EXEC)
	./$(EXEC)
