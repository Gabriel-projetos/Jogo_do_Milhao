
# Detecta o sistema operacional (Windows ou Linux)
ifdef OS
  OS := $(strip $(OS))
else
  OS := $(strip $(shell uname))
endif

# Nome base do executável (sem extensão)
BINNAME = jogo_do_milhao


# CONFIGURAÇÕES DE RAYLIB GLOBAL
ifeq ($(OS),Windows_NT)
	RAYLIB_PATH ?= C:/raylib
else
	RAYLIB_PATH ?= /usr/local
endif

# Inclusão e linkagem usando variável global RAYLIB_PATH
INCLUDE = -I$(RAYLIB_PATH)/include -I./Headers
LIBPATH = -L$(RAYLIB_PATH)/lib

# FLAGS E NOMES DE ARQUIVOS POR SO
ifeq ($(OS),Windows_NT)

	EXTRA_FLAGS = -Wall -Wextra -Wno-missing-braces -std=c99 -lraylib -lm -lopengl32 -lgdi32 -lwinmm
	BIN = $(BINNAME).exe
	RM = del /F /Q
	RUN = .\\$(BIN)

else

	EXTRA_FLAGS = -Wall -Wextra -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	BIN = ./$(BINNAME)
	RM = rm -f
	RUN = ./$(BIN)

endif

# FONTES DO PROJETO
SRC = main.c ./sources/*.c

# ALVOS 
all:
	@echo "Compilando $(BIN)..."
	gcc $(SRC) -g $(EXTRA_FLAGS) $(INCLUDE) $(LIBPATH) -o $(BIN)

run:
	@echo "Executando $(BIN)..."
	$(RUN)

debug:
	gdb $(BIN)

clean:
	@echo "Limpando..."
	-@$(RM) $(BIN)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BIN)

.PHONY: all clean run debug valgrind
