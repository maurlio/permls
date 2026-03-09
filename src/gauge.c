#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "gauge.h"

#define MAX_NAME_WIDTH 50
#define MIN_NAME_WIDTH 12

/**
 * Função de comparação personalizada para ordenar os arquivos por nome usando alphasort.
 */
static int custom_sort(const struct dirent **a, const struct dirent **b)
{
    return alphasort(b, a);
}

/**
 * Imprime o nome de um arquivo com codificação de cores opcional.
 */
static void print_colored_name(const char *name, mode_t mode, bool use_color, int width)
{
    const char *color = "";

    if (use_color)
    {
        if (S_ISDIR(mode))
            color = COLOR_BLUE;
        else if (mode & S_IXUSR)
            color = COLOR_GREEN;
    }

    printf("%s%-*.*s%s", color, width, width, name, (use_color ? COLOR_RESET : ""));
}

void display_file_info(const FileMeta *meta, const Options *opts, mode_t mode, int width, const char *parent_dir)
{
    char p_owner[PERMS_FORMAT_MAX_LEN], p_group[PERMS_FORMAT_MAX_LEN], p_other[PERMS_FORMAT_MAX_LEN];
    void (*fmt)(char *, size_t, bool, bool, bool) = opts->compact_mode ? perms_format_compact : perms_format_full;

    fmt(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
    fmt(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
    fmt(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

    printf("%-4s ", perms_get_file_type(mode));
    print_colored_name(meta->name, mode, opts->use_color, width);
    printf(" %s | %s | %s", p_owner, p_group, p_other);

    if (S_ISLNK(mode))
    {
        char full_path[MAX_PATH_LEN];
        char link_target[MAX_PATH_LEN];

        path_join(full_path, sizeof(full_path), parent_dir, meta->name);
        ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);

        if (len != -1)
        {
            link_target[len] = '\0';
            printf(" -> %s%s%s", COLOR_BLUE, link_target, opts->use_color ? COLOR_RESET : "");
        }
    }

    printf("\n");
}

int list_directory_contents(const char *directory, const Options *opts)
{
    struct dirent **nameList;
    int n = scandir(directory, &nameList, NULL, custom_sort);

    if (n < 0)
    {
        perror("scandir");
        return -1;
    }

    int width = MIN_NAME_WIDTH;
    for (int i = 0; i < n; i++)
    {
        int len = strlen(nameList[i]->d_name);
        if (len > width)
            width = (len > MAX_NAME_WIDTH) ? MAX_NAME_WIDTH : len;
    }

    struct stat statbuf;
    FileMeta meta;

    for (int i = 0; i < n; i++)
    {
        if (nameList[i]->d_name[0] == '.' && (nameList[i]->d_name[1] == '\0' || nameList[i]->d_name[1] == '.'))
        {
            free(nameList[i]);
            continue;
        }

        char path[MAX_PATH_LEN];
        path_join(path, sizeof(path), directory, nameList[i]->d_name);

        if (lstat(path, &statbuf) == 0)
        {
            strncpy(meta.name, nameList[i]->d_name, sizeof(meta.name) - 1);
            perms_extract(&statbuf, &meta.perms);
            display_file_info(&meta, opts, statbuf.st_mode, width, directory);
        }

        free(nameList[i]);
    }

    free(nameList);
    return 0;
}
