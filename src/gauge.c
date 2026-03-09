#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "gauge.h"
#include "utils.h"

#define W_TYPE 5
#define W_PERM 24
#define MAX_DYNAMIC_NAME_WIDTH 50
#define MIN_DYNAMIC_NAME_WIDTH 10

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

    if (use_color && strlen(color) > 0)
    {
        printf("%s%-*.*s%s", color, width, width, name, COLOR_RESET);
    }
    else
    {
        printf("%-*.*s", width, width, name);
    }
}

void display_file_info(const FileMeta *meta, const Options *opts, mode_t mode, int name_width)
{
    char p_owner[PERMS_FORMAT_MAX_LEN];
    char p_group[PERMS_FORMAT_MAX_LEN];
    char p_other[PERMS_FORMAT_MAX_LEN];

    // Preenchimento dos buffers de permissão
    if (opts->compact_mode)
    {
        perms_format_compact(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        perms_format_compact(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        perms_format_compact(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

        printf("%-4s ", S_ISDIR(mode) ? "dir" : "arq");
        print_colored_name(meta->name, mode, opts->use_color, name_width);
        printf(" %-10s %-10s %-10s\n", p_owner, p_group, p_other);
    }
    else
    {
        perms_format_full(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        perms_format_full(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        perms_format_full(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

        printf("%-4s ", S_ISDIR(mode) ? "dir" : "arq");
        print_colored_name(meta->name, mode, opts->use_color, name_width);
        printf(" %-*s %-*s %-s\n", W_PERM, p_owner, W_PERM, p_group, p_other);
    }
}

int list_directory_contents(const char *directory, const Options *opts)
{
    DIR *dirp = opendir(directory);
    if (!dirp)
    {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", directory, strerror(errno));
        return -1;
    }

    struct dirent *dp;
    int optimal_name_width = MIN_DYNAMIC_NAME_WIDTH;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        int len = (int)strlen(dp->d_name);
        if (len > optimal_name_width)
            optimal_name_width = len;
    }

    if (optimal_name_width > MAX_DYNAMIC_NAME_WIDTH)
        optimal_name_width = MAX_DYNAMIC_NAME_WIDTH;

    rewinddir(dirp);

    struct stat statbuf;
    FileMeta meta;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        if (path_join(full_path, sizeof(full_path), directory, dp->d_name) != 0)
            continue;

        if (lstat(full_path, &statbuf) == -1)
            continue;

        strncpy(meta.name, dp->d_name, sizeof(meta.name) - 1);
        meta.name[sizeof(meta.name) - 1] = '\0';

        perms_extract(&statbuf, &meta.perms);
        display_file_info(&meta, opts, statbuf.st_mode, optimal_name_width);
    }

    closedir(dirp);
    return 0;
}