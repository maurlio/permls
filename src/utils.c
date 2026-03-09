#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void init_options(Options *opts)
{
    strncpy(opts->directory, ".", MAX_PATH_LEN - 1);
    opts->directory[MAX_PATH_LEN - 1] = '\0';
    opts->compact_mode = false;
    opts->use_color = isatty(STDOUT_FILENO);
}

int path_join(char *dest, size_t size, const char *p1, const char *p2)
{
    size_t len1 = strlen(p1);

    // Verifica se p1 já termina com uma barra
    int has_slash = (len1 > 0 && p1[len1 - 1] == '/');

    // Formata o caminho completo
    int written = snprintf(dest, size, "%s%s%s", p1, has_slash ? "" : "/", p2);

    return (written < 0 || (size_t)written >= size) ? -1 : 0;
}

void display_help(void)
{
    printf("\nUso: permls [OPÇÕES]\n\n");
    printf("  --dir <path>    Diretório (Padrão: '.')\n");
    printf("  --compact       Modo enxuto\n");
    printf("  --no-color      Desativa cores\n");
}

int parse_arguments(int argc, char *argv[], Options *opts)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--dir") == 0 && i + 1 < argc)
        {
            strncpy(opts->directory, argv[++i], MAX_PATH_LEN - 1);
        }
        else if (strcmp(argv[i], "--compact") == 0)
        {
            opts->compact_mode = true;
        }
        else if (strcmp(argv[i], "--no-color") == 0)
        {
            opts->use_color = false;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            display_help();
            exit(0);
        }
        else
        {
            fprintf(stderr, "Argumento desconhecido: %s\n", argv[i]);
            display_help();
            return -1;
        }
    }
    return 0;
}