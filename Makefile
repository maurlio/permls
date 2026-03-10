CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L
SRCDIR = src
OBJDIR = build
TARGET = permls

SOURCES = $(SRCDIR)/utils.c $(SRCDIR)/perms.c $(SRCDIR)/gauge.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: prepare $(TARGET)

prepare:
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS) $(OBJDIR)/main.o
	$(CC) $(OBJECTS) $(OBJDIR)/main.o -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: prepare
	$(CC) $(CFLAGS) tests/unit_tests.c src/utils.c src/perms.c -o run_unit_tests
	./run_unit_tests
	@rm run_unit_tests
	@chmod +x tests/integration.sh
	./tests/integration.sh

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean prepare test