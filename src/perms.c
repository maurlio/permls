/**
 * @file perms.c
 * @brief Implementação para extração e formatação de permissões de arquivo (rwx).
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "perms.h"

// Buffer max. para strings de permissão formatadas
#define PERMS_FORMAT_MAX_LEN 128

// FUNÇÕES DE EXTRAÇÃO DE PERMISSÃO

/**
 * @brief Extrai os bits de permissão de `st->st_mode` para `FilePermissions`.
 */
void perms_extract(const struct stat *st, FilePermissions *fp)
{
    mode_t mode = st->st_mode;

    // OWNER (User)
    fp->owner_read = (mode & S_IRUSR) != 0;
    fp->owner_write = (mode & S_IWUSR) != 0;
    fp->owner_execute = (mode & S_IXUSR) != 0;

    // GROUP
    fp->group_read = (mode & S_IRGRP) != 0;
    fp->group_write = (mode & S_IWGRP) != 0;
    fp->group_execute = (mode & S_IXGRP) != 0;

    // OTHER
    fp->other_read = (mode & S_IROTH) != 0;
    fp->other_write = (mode & S_IWOTH) != 0;
    fp->other_execute = (mode & S_IXOTH) != 0;
}

/**
 * @brief Mapeia o modo do arquivo para uma string de tipo em Português (minúsculas).
 */
const char *perms_get_file_type(mode_t mode)
{
    if (S_ISREG(mode))
    {
        return "arquivo";
    }
    else if (S_ISDIR(mode))
    {
        return "diretório";
    }
    else if (S_ISLNK(mode))
    {
        return "link simbólico";
    }
    else if (S_ISCHR(mode))
    {
        return "dispositivo de caracteres";
    }
    else if (S_ISBLK(mode))
    {
        return "dispositivo de bloco";
    }
    else if (S_ISFIFO(mode))
    {
        return "fifo (pipe nomeado)";
    }
    else if (S_ISSOCK(mode))
    {
        return "socket";
    }
    else
    {
        return "desconhecido";
    }
}

// FUNÇÕES DE FORMATAÇÃO (DISPLAY)

/**
 * @brief Constrói a string de permissão no modo --full (descritivo, vírgulas).
 *
 * @note Aloca memória. O chamador é responsável por liberá-la.
 * @return String formatada (ex: "ler, escrever, -").
 */
char *perms_format_full(bool read, bool write, bool execute)
{
    char temp[PERMS_FORMAT_MAX_LEN] = {0};
    int offset = 0;

    if (read)
    {
        offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, "%s", PERM_READ);
    }
    if (write)
    {
        if (offset > 0)
            offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, ", ");
        offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, "%s", PERM_WRITE);
    }
    if (execute)
    {
        if (offset > 0)
            offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, ", ");
        offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, "%s", PERM_EXECUTE);
    }

    if (offset == 0)
    {
        offset += snprintf(temp + offset, PERMS_FORMAT_MAX_LEN - offset, "%s", PERM_NONE);
    }

    char *buffer = (char *)malloc(offset + 1);
    if (buffer == NULL)
        return NULL;
    strcpy(buffer, temp);
    return buffer;
}

/**
 * @brief Constrói a string de permissão no modo --compact.
 *
 * @note Aloca memória. O chamador é responsável por liberá-la.
 * @return String formatada (ex: "[l][-][x]").
 */
char *perms_format_compact(bool read, bool write, bool execute)
{
    char *buffer = (char *)malloc(PERMS_FORMAT_MAX_LEN);
    if (buffer == NULL)
        return NULL;

    // Constrói a string no formato: [l][e][x]
    snprintf(
        buffer,
        PERMS_FORMAT_MAX_LEN,
        "[%s][%s][%s]",
        read ? "l" : PERM_NONE,
        write ? "e" : PERM_NONE,
        execute ? "x" : PERM_NONE);

    return buffer;
}