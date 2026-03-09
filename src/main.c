#include <stdlib.h>
#include "gauge.h"

/**
 * @brief Função principal do programa.
 */
int main(int argc, char *argv[])
{
    Options opts;
    init_options(&opts);
    parse_arguments(argc, argv, &opts);

    if (list_directory_contents(opts.directory, &opts) != 0)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
