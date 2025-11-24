/**
 * @file utils.h
 * @brief Definições de estruturas e protótipos para manipulação de argumentos.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Tamanho máximo do caminho do diretório (valor POSIX comum)
#define MAX_PATH_LEN 4096

// ESTRUTURA DE OPÇÕES DO PROGRAMA

/**
 * @struct Options
 * @brief Configurações obtidas dos argumentos de linha de comando.
 */
typedef struct
{
    char directory[MAX_PATH_LEN]; // Caminho do diretório
    bool compact_mode;            // true para modo --compact (padrão é --full)
    bool show_meta;               // true se --meta foi especificado
    bool show_help;               // true se --help foi especificado
} Options;

// PROTÓTIPOS DE FUNÇÕES

/**
 * @brief Inicializa Options com defaults: dir=".", --full, sem --meta.
 *
 * @param opts Ponteiro para a estrutura Options.
 */
void init_options(Options *opts);

/**
 * @brief Analisa os argumentos de linha de comando.
 *
 * @param argc Número de argumentos.
 * @param argv Vetor de argumentos.
 * @param opts Ponteiro para Options onde as configs serão salvas.
 * @return 0 em sucesso, -1 em erro de argumento.
 */
int parse_arguments(int argc, char *argv[], Options *opts);

/**
 * @brief Exibe a mensagem de ajuda e uso.
 */
void display_help(void);

#endif