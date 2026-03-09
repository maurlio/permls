#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void init_options(Options *opts)
{
    strncpy(opts->directory, ".", MAX_PATH_LEN - 1);
    opts->directory[MAX_PATH_LEN - 1] = '\0';
    opts->compact_mode = false;
    opts->show_meta = false;
    opts->show_help = false;
}

int path_join(char *dest, size_t size, const char *p1, const char *p2)
{
    if (!dest || !p1 || !p2 || size == 0)
        return -1;

    size_t len1 = strlen(p1);

    // Verifica se p1 já termina com uma barra
    int has_slash = (len1 > 0 && p1[len1 - 1] == '/');

    // Formata o caminho completo
    int written = snprintf(dest, size, "%s%s%s", p1, has_slash ? "" : "/", p2);

    // Verifica se houve truncamento ou erro
    if (written < 0 || (size_t)written >= size)
    {
        return -1;
    }
    return 0;
}

void display_help(void)
{
    printf("\n--- permls: Visualizador de Permissões ---\n\n");
    printf("Uso: permls [OPÇÕES]\n\n");
    printf("Opções:\n");
    printf("  --dir <path>    Diretório a listar (Padrão: '.')\n");
    printf("  --compact       Modo de exibição enxuto\n");
    printf("  --meta          Exibe Inode e Tamanho\n");
    printf("  --help          Exibe esta ajuda\n");
}

int parse_arguments(int argc, char *argv[], Options *opts)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--dir") == 0 && i + 1 < argc)
        {
            strncpy(opts->directory, argv[++i], MAX_PATH_LEN - 1);
            opts->directory[MAX_PATH_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "--compact") == 0)
        {
            opts->compact_mode = true;
        }
        else if (strcmp(argv[i], "--meta") == 0)
        {
            opts->show_meta = true;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            opts->show_help = true;
            display_help();
        }
    }
    return 0;
}