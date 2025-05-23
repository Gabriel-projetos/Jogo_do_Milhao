# Nome do executavel fina
EXEC = jogo_do_milhao

# Compilador usado
cc = gcc

# Diretorios
src_dir = Sourcers
inc_dir = Headers

# Arquivos fonte
srcs = main.c $(src_dir)/pergunta.c $(src_dir)/funções_padrão.c

#Flags
cflags = -I$(inc_dir) -Wall

# Regra padrão
all: $(EXEC)

# Compila tudo
$(EXEC): $(srcs)
	$(cc) $(srcs) $(cflags) -o $(EXEC)

# Limpa os arquivos gerados
clean:
	del /f /q $(EXEC).exe 2>nul

# Executa
run: $(EXEC)
	./$(EXEC)
