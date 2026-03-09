#ifndef PERMS_H
#define PERMS_H

#include <sys/stat.h>
#include <stdbool.h>
#include <stddef.h>

// Definições de tamanho para segurança de buffer
#define PERMS_FORMAT_MAX_LEN 128

// Palavras para as permissões (Tradução)
#define PERM_READ "ler"
#define PERM_WRITE "escrever"
#define PERM_EXECUTE "executar"
#define PERM_NONE "-" 

typedef struct {
    bool owner_read, owner_write, owner_execute;
    bool group_read, group_write, group_execute;
    bool other_read, other_write, other_execute;
} FilePermissions;

typedef struct {
    char name[256];
    FilePermissions perms;
} FileMeta;

/**
 * @brief Extrai bits de permissão de mode_t para a estrutura FilePermissions.
 */
void perms_extract(const struct stat *st, FilePermissions *fp);

/**
 * @brief Retorna o tipo de arquivo traduzido (ex: "diretório").
 */
const char *perms_get_file_type(mode_t mode);

/**
 * @brief Formata permissões no modo completo (ex: "ler, escrever").
 */
void perms_format_full(char *dest, size_t size, bool r, bool w, bool x);

/**
 * @brief Formata permissões no modo compacto (ex: [l][e][-]).
 */
void perms_format_compact(char *dest, size_t size, bool r, bool w, bool x);

#endif