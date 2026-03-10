#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_PATH 4096
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

typedef struct
{
    char target_dir[MAX_PATH];
    bool use_color;
} Config;

/**
 * Inicializa a estrutura de opções com valores padrão.
 */
void config_init(Config *cfg);

/**
 * Analisa os argumentos da linha de comando e preenche a estrutura de opções.
 */
int config_parse_args(int argc, char *argv[], Config *cfg);

/**
 * Formata um tamanho em bytes para uma representação mais legível (B, K, M, G, T).
 */
void format_human_size(char *dest, long long bytes);

/**
 * Une dois componentes de um caminho de forma segura.
 */
int path_combine(char *dest, size_t size, const char *p1, const char *p2);

#endif