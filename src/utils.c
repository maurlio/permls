#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void config_init(Config *cfg)
{
    strncpy(cfg->target_dir, ".", MAX_PATH - 1);
    cfg->use_color = isatty(STDOUT_FILENO);
}

int config_parse_args(int argc, char *argv[], Config *cfg)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--dir") == 0 && i + 1 < argc)
        {
            strncpy(cfg->target_dir, argv[++i], MAX_PATH - 1);
        }
        else if (strcmp(argv[i], "--no-color") == 0)
        {
            cfg->use_color = false;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            printf("Uso: permls [--dir <path>] [--no-color]\n");
            exit(0);
        }
    }
    return 0;
}

void format_human_size(char *dest, long long bytes)
{
    const char *units[] = {"B", "K", "M", "G", "T"};
    int i = 0;
    double size = (double)bytes;

    while (size >= 1024 && i < 4)
    {
        size /= 1024;
        i++;
    }

    sprintf(dest, i == 0 ? "%.0f%s" : "%.1f%s", size, units[i]);
}

int path_combine(char *dest, size_t size, const char *p1, const char *p2)
{
    size_t len1 = strlen(p1);
    int has_slash = (len1 > 0 && p1[len1 - 1] == '/');
    return snprintf(dest, size, "%s%s%s", p1, has_slash ? "" : "/", p2) >= (int)size ? -1 : 0;
}
