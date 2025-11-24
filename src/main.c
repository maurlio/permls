/**
 * @file main.c
 * @brief Ponto de entrada principal para a ferramenta permls.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Incluir headers dos módulos
#include "gauge.h"
#include "perms.h"
#include "utils.h"

// Protótipo da função de execução
static int run_permls(const char *directory, const Options *opts);

/**
 * @brief Função principal do programa.
 */
int main(int argc, char *argv[])
{
    // Inicializa opções padrão
    Options opts;
    init_options(&opts);

    // Analisa argumentos de linha de comando
    if (parse_arguments(argc, argv, &opts) != 0)
    {
        // parse_arguments trata e exibe erros/ajuda
        return EXIT_FAILURE;
    }

    // Termina se --help foi solicitado
    if (opts.show_help)
    {
        return EXIT_SUCCESS;
    }

    // Executa a listagem principal
    int result = run_permls(opts.directory, &opts);

    return (result == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/**
 * @brief Função de execução principal do permls.
 *
 * Delega a listagem ao módulo gauge.
 *
 * @param directory Caminho do diretório.
 * @param opts Opções de execução.
 * @return 0 em sucesso, -1 em erro.
 */
static int run_permls(const char *directory, const Options *opts)
{
    // Chama o módulo gauge para a listagem
    return list_directory_contents(directory, opts);
}