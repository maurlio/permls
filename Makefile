# Compilador
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L

# Diretórios
SRCDIR = src
INCDIR = include
OBJDIR = build

# Executável
EXECUTABLE = permls
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: $(OBJDIR) $(EXECUTABLE)

# Cria o diretório build
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Regra de linkagem: Cria o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ 

# Regra de compilação: Compila cada arquivo C para .o na pasta build
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza: remove objetos e o executável
.PHONY: clean
clean:
	@rm -f $(OBJDIR)/*.o $(EXECUTABLE)

# Checagem de memória com Valgrind
.PHONY: memcheck
memcheck: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE) --dir .

TEST_DIR = tests
TEST_SRC = $(TEST_DIR)/test_perms.c
TEST_EXE = run_tests

# Compila e executa os testes
test: $(SRCDIR)/perms.c $(INCDIR)/perms.h
	$(CC) $(CFLAGS) $(TEST_SRC) $(SRCDIR)/perms.c -o $(TEST_EXE)
	@./$(TEST_EXE)
	@rm -f $(TEST_EXE)

.PHONY: all clean test