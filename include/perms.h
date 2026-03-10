#ifndef PERMS_H
#define PERMS_H

#include <sys/stat.h>
#include <stdbool.h>

typedef struct
{
    bool read, write, execute;
} AccessLevel;

typedef struct
{
    AccessLevel user, group, other;
} FileSecurity;

/**
 * Extrai os bits de permissão do sistema para a estrutura interna
 */
void security_extract(mode_t mode, FileSecurity *security);

/**
 * Retorna uma string representando o tipo (ex: "dir", "link", "arq").
 */
const char* security_get_type_label(mode_t mode);

/**
 * Formata o trio de permissões (ex: rwx ou r--)
 */
void security_format_triplet(char *dest, AccessLevel level);

#endif