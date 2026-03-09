#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "gauge.h"

#define W_TYPE 5
#define W_NAME 20
#define W_PERM 24

static const char *get_short_type(const char *full_type)
{
    if (strcmp(full_type, "diretório") == 0)
        return "dir";
    if (strcmp(full_type, "arquivo") == 0)
        return "arq";
    return "out";
}

static void print_truncated(const char *text, int width)
{
    printf("%-*.*s", width, width, text);
}

void display_file_info(const FileMeta *meta, const Options *opts)
{
    char p_owner[PERMS_FORMAT_MAX_LEN];
    char p_group[PERMS_FORMAT_MAX_LEN];
    char p_other[PERMS_FORMAT_MAX_LEN];

    if (opts->compact_mode)
    {
        perms_format_compact(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        perms_format_compact(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        perms_format_compact(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

        printf("%-*s ", W_TYPE, get_short_type(meta->type));
        print_truncated(meta->name, W_NAME);
        printf(" %-10s %-10s %-10s\n", p_owner, p_group, p_other);
    }
    else
    {
        perms_format_full(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
        perms_format_full(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
        perms_format_full(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

        printf("%-*s ", W_TYPE, get_short_type(meta->type));
        print_truncated(meta->name, W_NAME);
        printf(" %-*s %-*s %-s\n", W_PERM, p_owner, W_PERM, p_group, p_other);
    }
}

int list_directory_contents(const char *directory, const Options *opts)
{
    DIR *dirp = opendir(directory);
    if (!dirp)
        return -1;

    struct dirent *dp;
    struct stat statbuf;
    FileMeta meta;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        snprintf(full_path, MAX_PATH_LEN, "%s/%s", directory, dp->d_name);

        if (lstat(full_path, &statbuf) == -1)
            continue;

        strncpy(meta.name, dp->d_name, sizeof(meta.name) - 1);
        meta.name[sizeof(meta.name) - 1] = '\0';

        strncpy(meta.type, perms_get_file_type(statbuf.st_mode), sizeof(meta.type) - 1);
        meta.size = (long long)statbuf.st_size;
        meta.inode = (long)statbuf.st_ino;
        perms_extract(&statbuf, &meta.perms);

        display_file_info(&meta, opts);
    }

    closedir(dirp);
    return 0;
}