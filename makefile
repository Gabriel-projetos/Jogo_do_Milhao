# Nome do executável final
EXEC = jogo_do_milhao.exe

# Compilador usado
CC = gcc

# Diretórios da raylib
RAYLIB_INC = C:/raylib/include
RAYLIB_LIB = C:/raylib/lib

# Diretórios do seu projeto
SRC_DIR = sources
INC_DIR = Headers

# Arquivos fonte do projeto: main.c, os .c em 'sources/', e o novo telas_jogo.c
SRCS = main.c \
       $(SRC_DIR)/pergunta.c \
       $(SRC_DIR)/funcoes_padrao.c \
       $(SRC_DIR)/leitor_csv.c \
       $(SRC_DIR)/jogo.c \
       $(SRC_DIR)/telas_jogo.c 
	   
# Arquivos objeto (.o) gerados a partir dos arquivos-fonte
OBJS = $(patsubst %.c,%.o,$(SRCS))

# Flags do compilador
# -I. : Adiciona o diretório atual (onde telas_jogo.h está, se estiver na raiz) aos caminhos de busca de cabeçalhos.
CFLAGS = -I$(INC_DIR) -I$(RAYLIB_INC) -I. -Wall -Wextra -std=c99

# Flags de linkagem
LDFLAGS = -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm -pthread

# Regra padrão: compila tudo e cria o executável
all: $(EXEC)

# Regra para linkar o executável
$(EXEC): $(OBJS)
	@echo "LINKING $(EXEC)..."
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXEC)

# Regra genérica para compilar arquivos .c (na raiz do projeto, como main.c) em arquivos .o
%.o: %.c
	@echo "COMPILING $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar arquivos .c de subdiretórios (ex: sources/) em arquivos .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "COMPILING $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa o executável e todos os arquivos objeto
clean:
	@echo "CLEANING project..."
	-@del /f /q $(EXEC) $(OBJS) 2>nul || rm -f $(EXEC) $(OBJS)

# Executa o programa
run: $(EXEC)
	@echo "RUNNING $(EXEC)..."
	./$(EXEC)

.PHONY: all clean run
