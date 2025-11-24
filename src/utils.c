/**
 * @file utils.c
 * @brief Implementação das funções de utilidade (argumentos e ajuda).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * @brief Inicializa a estrutura Options com valores padrão (dir=".", --full, sem --meta).
 */
void init_options(Options *opts)
{
    // Diretório padrão: o diretório atual
    strncpy(opts->directory, ".", MAX_PATH_LEN - 1);
    opts->directory[MAX_PATH_LEN - 1] = '\0'; // Garante terminação nula

    opts->compact_mode = false;
    opts->show_meta = false;
    opts->show_help = false;
}

/**
 * @brief Exibe a mensagem de ajuda e uso do programa.
 */
void display_help(void)
{
    printf("\n");
    printf("--- permls: Visualizador Didático de Permissões Linux ---\n");
    printf("\n");
    printf("Uso: permls [OPÇÕES]\n");
    printf("\n");
    printf("Lista arquivos e exibe permissões de forma didática em Português.\n");
    printf("\n");
    printf("Parâmetros de Linha de Comando:\n");
    printf("  --dir <diretório>   Diretório a ser listado. Padrão: diretório atual ('.').\n");
    printf("  --full              Modo descritivo (Padrão). Ex: OWNER: LER / ESCREVER / EXECUTAR.\n");
    printf("  --compact           Modo compacto. Ex: OWNER[L][W][X].\n");
    printf("  --meta              Inclui metadados adicionais (tamanho, inode, tipo de arquivo).\n");
    printf("  --help              Exibe esta mensagem de ajuda.\n");
    printf("\n");
    printf("Exemplos:\n");
    printf("  permls --dir /etc --compact\n");
    printf("  permls --meta\n");
    printf("\n");
}

/**
 * @brief Analisa os argumentos de linha de comando e popula a estrutura Options.
 */
int parse_arguments(int argc, char *argv[], Options *opts)
{
    if (argc < 2)
    {
        // Usa padrões se nenhuma opção foi passada.
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            opts->show_help = true;
            display_help();
            return 0; // Sai imediatamente se --help for pedido
        }

        else if (strcmp(argv[i], "--dir") == 0)
        {
            if (i + 1 < argc)
            {
                // Copia o próximo argumento (o caminho)
                strncpy(opts->directory, argv[i + 1], MAX_PATH_LEN - 1);
                opts->directory[MAX_PATH_LEN - 1] = '\0';
                i++; // Pula o argumento de caminho
            }
            else
            {
                fprintf(stderr, "ERRO: O argumento --dir requer um caminho.\n");
                return -1;
            }
        }

        else if (strcmp(argv[i], "--compact") == 0)
        {
            if (opts->compact_mode == false)
            {
                // Ativa o modo compacto
                opts->compact_mode = true;
            }
            else
            {
                fprintf(stderr, "AVISO: --compact especificado mais de uma vez. Ignorando repetição.\n");
            }
        }

        else if (strcmp(argv[i], "--full") == 0)
        {
            // Tratamento de conflito: --full é o padrão.
            if (opts->compact_mode == true)
            {
                fprintf(stderr, "AVISO: --full e --compact são mutuamente exclusivos. Mantendo o modo --compact.\n");
            }
            // else: mantemos o padrão (full)
        }

        else if (strcmp(argv[i], "--meta") == 0)
        {
            opts->show_meta = true;
        }

        else
        {
            fprintf(stderr, "ERRO: Argumento desconhecido: %s\n", argv[i]);
            fprintf(stderr, "Use 'permls --help' para ver as opções disponíveis.\n");
            return -1;
        }
    }

    return 0; // Sucesso na análise
}