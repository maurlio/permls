/**
 * @file gauge.c
 * @brief Implementação da listagem de diretório com formatação para display.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "gauge.h"
#include "perms.h"
#include "utils.h"

// Larguras de coluna para alinhamento em terminal
#define W_TYPE 5
#define W_INODE 8
#define W_SIZE 8
#define W_NAME 20 // Nome truncado visualmente
#define W_PERM 24 // Largura para modo --full

// FUNÇÕES AUXILIARES

/**
 * @brief Retorna uma versão curta do tipo para display compacto.
 */
const char *get_short_type(const char *full_type)
{
    if (strcmp(full_type, "diretório") == 0)
        return "dir";
    if (strcmp(full_type, "arquivo") == 0)
        return "arq";
    if (strcmp(full_type, "link simbólico") == 0)
        return "link";
    return "out";
}

/**
 * @brief Imprime uma string, truncando se exceder a largura máxima.
 */
void print_truncated(const char *text, int width)
{
    // %-*.*s alinha à esquerda, imprime no máximo 'width' caracteres
    printf("%-*.*s ", width, width, text);
}

// FUNÇÕES DE EXIBIÇÃO

/**
 * @brief Exibe as informações e permissões formatadas de um arquivo.
 */
void display_file_info(const FileMeta *meta, const Options *opts)
{
    char *p_owner = NULL;
    char *p_group = NULL;
    char *p_other = NULL;

    // Formatar Permissões (Modo Compacto vs. Full)
    if (opts->compact_mode)
    {
        p_owner = perms_format_compact(meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        p_group = perms_format_compact(meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        p_other = perms_format_compact(meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);
    }
    else
    {
        p_owner = perms_format_full(meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        p_group = perms_format_full(meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        p_other = perms_format_full(meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);
    }

    if (!p_owner || !p_group || !p_other)
        goto cleanup;

    // Colunas Iniciais (Tipo, Meta, Nome)
    const char *short_type = get_short_type(meta->type);

    if (opts->show_meta)
    {
        // Exibe colunas extras (inode, tamanho)
        printf("%-*s %*ld %*lld ", W_TYPE, short_type, W_INODE, meta->inode, W_SIZE, meta->size);
    }
    else
    {
        // Modo Padrão (minimalista)
        printf("%-*s ", W_TYPE, short_type);
    }

    // Nome (Truncado)
    print_truncated(meta->name, W_NAME);

    // Colunas de Permissão
    if (opts->compact_mode)
    {
        // Compacto: [l][e][x]
        printf("%-9s  %-9s  %-9s\n", p_owner, p_group, p_other);
    }
    else
    {
        // Full: ler, escrever, executar
        printf("%-*s %-*s %-s\n",
               W_PERM, p_owner,
               W_PERM, p_group,
               p_other); // Sem padding na última coluna
    }

cleanup:
    // Libera a memória alocada nas funções de formatação
    free(p_owner);
    free(p_group);
    free(p_other);
}

// FUNÇÃO DE LISTAGEM PRINCIPAL

/**
 * @brief Abre, itera e exibe o conteúdo de um diretório, chamando stat/lstat para cada item.
 */
int list_directory_contents(const char *directory, const Options *opts)
{
    DIR *dirp = opendir(directory);
    if (!dirp)
    {
        fprintf(stderr, "erro: não foi possível abrir '%s': %s\n", directory, strerror(errno));
        return -1;
    }

    // --- CABEÇALHO ---
    if (opts->show_meta)
    {
        printf("%-*s %*s %*s ", W_TYPE, "tipo", W_INODE, "inode", W_SIZE, "tam");
    }
    else
    {
        printf("%-*s ", W_TYPE, "tipo");
    }

    printf("%-*s ", W_NAME, "nome");

    if (opts->compact_mode)
    {
        printf("%-9s  %-9s  %-9s\n", "dono", "grupo", "outros");
    }
    else
    {
        printf("%-*s %-*s %-s\n", W_PERM, "dono", W_PERM, "grupo", "outros");
    }

    // Linha separadora
    int line_len = 80;
    if (!opts->compact_mode)
    {
        line_len = W_TYPE + 1 + W_NAME + 1 + (W_PERM * 3) + 2;
        if (opts->show_meta)
            line_len += W_INODE + 1 + W_SIZE + 1;
    }

    for (int i = 0; i < line_len && i < 120; i++)
        printf("-");
    printf("\n");
    // -----------------

    struct dirent *dp;
    struct stat statbuf;
    FileMeta meta;

    while ((dp = readdir(dirp)) != NULL)
    {
        // Ignora os diretórios '.' e '..'
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", directory, dp->d_name);

        // Usa lstat para não seguir links simbólicos
        if (lstat(full_path, &statbuf) == -1)
            continue;

        // Popula a estrutura FileMeta
        strncpy(meta.name, dp->d_name, sizeof(meta.name) - 1);
        meta.name[sizeof(meta.name) - 1] = '\0';
        meta.size = (long long)statbuf.st_size;
        meta.inode = (long)statbuf.st_ino;

        const char *ft = perms_get_file_type(statbuf.st_mode);
        strncpy(meta.type, ft, sizeof(meta.type) - 1);
        meta.type[sizeof(meta.type) - 1] = '\0';

        perms_extract(&statbuf, &meta.perms);
        display_file_info(&meta, opts);
    }

    closedir(dirp);
    return 0;
}