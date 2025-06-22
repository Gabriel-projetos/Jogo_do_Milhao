# Nome do executável final
EXEC = jogo_do_milhao

# Compilador usado
CC = gcc

# Diretórios do projeto
SRC_DIR = sources
INC_DIR = Headers

# Diretórios da raylib (alterar de acordo com o sistema)
RAYLIB_INC = C:/raylib/include
RAYLIB_LIB = C:/raylib/lib

# Arquivos fonte
SRCS = main.c $(SRC_DIR)/pergunta.c $(SRC_DIR)/funcoes_padrao.c $(SRC_DIR)/leitor_csv.c $(SRC_DIR)/jogo.c

# Flags do compilador
CFLAGS = -I$(INC_DIR) -I$(RAYLIB_INC) -Wall -Wextra

# Flags de link com a raylib
LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm

# Regra padrão
all: $(EXEC)

# Como compilar o executável
$(EXEC): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o $(EXEC)

# Limpa o executável (Windows e Unix compatível)
clean:
	-@del /f /q $(EXEC).exe 2>nul || rm -f $(EXEC)

# Executa o programa
run: $(EXEC)
	./$(EXEC)
