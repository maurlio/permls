#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Tamanho máximo para caminhos de ficheiros.
 */
#define MAX_PATH_LEN 4096

/**
 * @brief Códigos de cores ANSI para formatação de saída no terminal.
 */
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

/**
 * @brief Estrutura para armazenar as opções de configuração do programa.
 */
typedef struct
{
    char directory[MAX_PATH_LEN];
    bool compact_mode;
    bool use_color;
} Options;

/**
 * @brief Inicializa a estrutura de opções com valores padrão.
 * @param opts Ponteiro para a estrutura de opções a ser inicializada.
 */
void init_options(Options *opts);

/**
 * @brief Analisa os argumentos da linha de comando e preenche a estrutura de opções.
 * @param argc Número de argumentos.
 * @param argv Array de strings contendo os argumentos.
 * @param opts Ponteiro para a estrutura de opções a ser preenchida.
 */
int parse_arguments(int argc, char *argv[], Options *opts);

/**
 * @brief Exibe a mensagem de ajuda com informações sobre o uso do programa e suas opções.
 */
void display_help(void);

/**
 * @brief Une dois componentes de um caminho de forma segura.
 * @param dest Buffer de destino.
 * @param size Tamanho do buffer de destino.
 * @param p1 Primeiro componente (diretório).
 * @param p2 Segundo componente (nome do ficheiro).
 */
int path_join(char *dest, size_t size, const char *p1, const char *p2);

/**
 * @brief Formata um tamanho em bytes para uma representação mais legível (B, K, M, G, T).
 * @param dest Buffer de destino.
 * @param size Tamanho do buffer de destino.
 * @param bytes Tamanho em bytes a ser formatado.
 */
void format_size_human(char *dest, size_t size, long long bytes);

#endif