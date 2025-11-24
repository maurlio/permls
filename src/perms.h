/**
 * @file perms.h
 * @brief Definições de estruturas e protótipos para interpretação e tradução
 * das permissões de arquivo (rwx).
 */

#ifndef PERMS_H
#define PERMS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

// Palavras para as permissões
#define PERM_READ "ler"
#define PERM_WRITE "escrever"
#define PERM_EXECUTE "executar"
#define PERM_NONE "-" // Caractere para permissão ausente

// ESTRUTURAS

/**
 * @struct FilePermissions
 * @brief Status booleano de permissão (read/write/execute) por categoria
 * (Owner, Group, Other).
 */
typedef struct
{
    // OWNER
    bool owner_read;
    bool owner_write;
    bool owner_execute;

    // GROUP
    bool group_read;
    bool group_write;
    bool group_execute;

    // OTHER
    bool other_read;
    bool other_write;
    bool other_execute;
} FilePermissions;

/**
 * @struct FileMeta
 * @brief Metadados e permissões de um arquivo.
 */
typedef struct
{
    char name[256];        // Nome do arquivo
    char type[32];         // Tipo do arquivo (ex: "arquivo", "diretorio")
    long long size;        // Tamanho em bytes
    long inode;            // Número do Inode
    FilePermissions perms; // Permissões extraídas
} FileMeta;

// PROTÓTIPOS DE FUNÇÕES

/**
 * @brief Extrai os bits de permissão de `stat` para `FilePermissions`.
 *
 * Mapeia bits S_IRUSR, S_IWUSR, etc., para a estrutura booleana.
 *
 * @param st Ponteiro para a estrutura stat.
 * @param fp Ponteiro para a estrutura FilePermissions de destino.
 */
void perms_extract(const struct stat *st, FilePermissions *fp);

/**
 * @brief Determina o tipo de arquivo a partir de `mode_t` e retorna o nome em Português.
 *
 * @param mode O campo st_mode da estrutura stat.
 * @return String constante (char*) representando o tipo (ex: "diretório").
 */
const char *perms_get_file_type(mode_t mode);

/**
 * @brief Constrói a string de permissão no modo --full (descritivo).
 *
 * @param read Status de LER.
 * @param write Status de ESCREVER.
 * @param execute Status de EXECUTAR.
 * @return String formatada (ex: "ler, escrever, -"). (Alocada dinamicamente)
 */
char *perms_format_full(bool read, bool write, bool execute);

/**
 * @brief Constrói a string de permissão no modo --compact.
 *
 * @param read Status de LER.
 * @param write Status de ESCREVER.
 * @param execute Status de EXECUTAR.
 * @return String formatada (ex: "[l][e][x]"). (Alocada dinamicamente)
 */
char *perms_format_compact(bool read, bool write, bool execute);

#endif // PERMS_H