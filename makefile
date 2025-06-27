# Detecta o sistema operacional
ifdef OS
  OS := $(strip $(OS))
else
  OS := $(strip $(shell uname))
endif

# Nome do executável
BINNAME = Jogo_do_Milhao

# Arquivo final
ifeq ($(OS),Windows_NT)
	INCLUDE = -I./include -I./Headers -L./libwin
	EXTRA_FLAGS = -Wall -Wextra -Wno-missing-braces -std=c99 -l:libRaylib.a -lopengl32 -lgdi32 -lwinmm
	BIN = $(BINNAME).exe
	RM = del /F /Q
	RUN = .\\$(BIN)
else
	INCLUDE = -I./include -I./Headers -L./lib
	EXTRA_FLAGS = -Wall -Wextra -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	BIN = ./$(BINNAME)
	RM = rm -f
	RUN = ./$(BIN)
endif

# Fontes
SRC = main.c ./sources/*.c

all:
	@echo "Compilando $(BIN)..."
	gcc $(SRC) -g $(EXTRA_FLAGS) $(INCLUDE) -o $(BIN)

run:
	@echo "Executando..."
	$(RUN)

debug:
	gdb $(BIN)

clean:
	@echo "Limpando..."
	-@$(RM) $(BIN)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BIN)

.PHONY: all clean run debug valgrind
