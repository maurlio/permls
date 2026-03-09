#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "gauge.h"

#define MAX_NAME_WIDTH 50
#define MIN_NAME_WIDTH 10

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

void display_file_info(const FileMeta *meta, const Options *opts, mode_t mode, int width)
{
    char p_owner[PERMS_FORMAT_MAX_LEN];
    char p_group[PERMS_FORMAT_MAX_LEN];
    char p_other[PERMS_FORMAT_MAX_LEN];

    void (*fmt)(char *, size_t, bool, bool, bool) = opts->compact_mode ? perms_format_compact : perms_format_full;

    fmt(p_owner, sizeof(p_owner), meta->perms.owner_read, meta->perms.owner_write, meta->perms.owner_execute);
    fmt(p_group, sizeof(p_group), meta->perms.group_read, meta->perms.group_write, meta->perms.group_execute);
    fmt(p_other, sizeof(p_other), meta->perms.other_read, meta->perms.other_write, meta->perms.other_execute);

    printf("%-4s ", perms_get_file_type(mode));
    print_colored_name(meta->name, mode, opts->use_color, width);
    printf(" %s | %s | %s\n", p_owner, p_group, p_other);
}

int list_directory_contents(const char *directory, const Options *opts)
{
    DIR *dir = opendir(directory);
    if (!dir)
        return -1;

    struct dirent *ent;
    int width = MIN_NAME_WIDTH;

    while ((ent = readdir(dir)))
    {
        int len = strlen(ent->d_name);
        if (len > width)
            width = len;
    }

    if (width > MAX_NAME_WIDTH)
        width = MAX_NAME_WIDTH;

    rewinddir(dir);

    struct stat statbuf;
    FileMeta meta;

    while ((ent = readdir(dir)))
    {
        if (ent->d_name[0] == '.' && (ent->d_name[1] == '\0' || ent->d_name[1] == '.'))
            continue;

        char path[MAX_PATH_LEN];
        if (path_join(path, sizeof(path), directory, ent->d_name) != 0)
            continue;

        if (lstat(path, &statbuf) == -1)
            continue;

        strncpy(meta.name, ent->d_name, sizeof(meta.name) - 1);
        perms_extract(&statbuf, &meta.perms);
        display_file_info(&meta, opts, statbuf.st_mode, width);
    }

    closedir(dir);
    return 0;
}
