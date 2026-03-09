#ifndef PERMS_H
#define PERMS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

// Definições de tamanho para segurança de buffer
#define PERMS_FORMAT_MAX_LEN 128
#define PERMS_COMPACT_MAX_LEN 16

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
    char type[32];
    long long size;
    long inode;
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
 * @brief Formata permissões no modo por extenso (ex: "ler, escrever").
 * @param dest Buffer de destino provido pelo chamador.
 * @param size Tamanho do buffer de destino.
 */
void perms_format_full(char *dest, size_t size, bool read, bool write, bool execute);

/**
 * @brief Formata permissões no modo compacto (ex: "[l][e][x]").
 */
void perms_format_compact(char *dest, size_t size, bool read, bool write, bool execute);

#endif