#include <stdio.h>
#include <stdlib.h>
#include "gauge.h"

int main(int argc, char *argv[])
{
    Config cfg;
    config_init(&cfg);

    if (config_parse_args(argc, argv, &cfg) != 0)
    {
        return EXIT_FAILURE;
    }

    if (core_list_directory(&cfg) != 0)
    {
        perror("Erro ao acessar diretório");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
