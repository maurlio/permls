# Compilador C
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11 -pedantic -Iinclude

# Diretórios
SRCDIR = src
BUILDDIR = build
INCLUDEDIR = include

# Executável (na raiz do projeto)
EXECUTABLE = permls

# Arquivos fonte C
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Arquivos objeto (.o) na pasta build
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

# Alvo principal: cria a pasta build, compila e linka o executável
all: $(BUILDDIR) $(EXECUTABLE)

# Cria o diretório build
$(BUILDDIR):
	@mkdir -p $@

# Regra de linkagem: Cria o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@

# Regra de compilação: Compila cada arquivo C para .o na pasta build
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza: remove objetos e o executável
.PHONY: clean
clean:
	@rm -f $(BUILDDIR)/*.o $(EXECUTABLE)
	@rmdir $(BUILDDIR) 2>/dev/null || true