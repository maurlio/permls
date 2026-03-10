#undef _POSIX_C_SOURCE 
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include "utils.h"
#include "perms.h"

void test_format_human_size()
{
    char buf[12];

    format_human_size(buf, 500);
    assert(strcmp(buf, "500B") == 0);

    format_human_size(buf, 1024);
    assert(strcmp(buf, "1.0K") == 0);

    format_human_size(buf, 1048576);
    assert(strcmp(buf, "1.0M") == 0);

    printf("[PASS] format_human_size\n");
}

void test_path_combine()
{
    char buf[MAX_PATH];

    path_combine(buf, sizeof(buf), "/tmp", "file.txt");
    assert(strcmp(buf, "/tmp/file.txt") == 0);

    path_combine(buf, sizeof(buf), "/tmp/", "file.txt");
    assert(strcmp(buf, "/tmp/file.txt") == 0);

    printf("[PASS] path_combine\n");
}

void test_security_labels()
{
    assert(strcmp(security_get_type_label(S_IFDIR), "dir") == 0);
    assert(strcmp(security_get_type_label(S_IFREG), "arq") == 0);
    printf("[PASS] security_labels\n");
}

int main()
{
    printf("Executando Testes Unitários\n");
    test_format_human_size();
    test_path_combine();
    test_security_labels();
    printf("Todos os testes unitários passaram!\n");
    return 0;
}