#include <stdio.h>
#include <string.h>
#include "perms.h"

// Função auxiliar para evitar repetição de lógica de erro
int assert_str(const char *label, const char *expected, const char *actual)
{
    if (strcmp(expected, actual) != 0)
    {
        printf("FALHA [%s]: Esperado '%s', recebido '%s'\n", label, expected, actual);
        return 1;
    }
    printf("OK [%s]\n", label);
    return 0;
}

int main()
{
    int errors = 0;
    char buf[PERMS_FORMAT_MAX_LEN];

    printf("Iniciando testes: perms\n");

    // Testes: perms_format_full
    perms_format_full(buf, sizeof(buf), true, true, true);
    errors += assert_str("Full 7", "ler, escrever, executar", buf);

    perms_format_full(buf, sizeof(buf), true, false, false);
    errors += assert_str("Full 4", "ler", buf);

    perms_format_full(buf, sizeof(buf), false, false, false);
    errors += assert_str("Full 0", "-", buf);

    // Testes: perms_format_compact
    perms_format_compact(buf, sizeof(buf), true, true, false);
    errors += assert_str("Compact 6", "[l][e][-]", buf);

    perms_format_compact(buf, sizeof(buf), false, false, true);
    errors += assert_str("Compact 1", "[-][-][x]", buf);

    // Resultado Final
    if (errors == 0)
    {
        printf("Sucesso: Todos os testes passaram\n");
    }
    else
    {
        printf("Erro: %d falhas encontradas\n", errors);
    }

    return errors > 0 ? 1 : 0;
}