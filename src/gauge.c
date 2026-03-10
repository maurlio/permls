#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "gauge.h"

static void render_row(const FileRecord *file, int padding, bool color_enabled)
{
    char u_str[4], g_str[4], o_str[4], size_str[12];

    format_human_size(size_str, file->size);
    security_format_triplet(u_str, file->security.user);
    security_format_triplet(g_str, file->security.group);
    security_format_triplet(o_str, file->security.other);

    printf("%-4s ", security_get_type_label(file->mode));

    if (color_enabled)
    {
        const char *color = S_ISDIR(file->mode) ? COLOR_BLUE : (file->mode & S_IXUSR) ? COLOR_GREEN
                                                                                      : "";
        printf("%s%s%s", color, file->name, COLOR_RESET);
    }
    else
    {
        printf("%s", file->name);
    }

    int spaces = padding - (int)strlen(file->name);
    for (int i = 0; i < spaces; i++)
        putchar(' ');

    printf("  %6s  |  u:%s g:%s o:%s\n", size_str, u_str, g_str, o_str);
}

int core_list_directory(const Config *cfg)
{
    struct dirent **entries;

    int count = scandir(cfg->target_dir, &entries, NULL, alphasort);
    if (count < 0)
        return -1;

    int max_name_len = 12;
    for (int i = 0; i < count; i++)
    {
        int len = strlen(entries[i]->d_name);
        if (len > max_name_len)
            max_name_len = len;
    }

    for (int i = 0; i < count; i++)
    {
        if (entries[i]->d_name[0] == '.')
        {
            free(entries[i]);
            continue;
        }

        struct stat st;
        char full_path[MAX_PATH];
        path_combine(full_path, sizeof(full_path), cfg->target_dir, entries[i]->d_name);

        if (lstat(full_path, &st) == 0)
        {
            FileRecord record;
            strncpy(record.name, entries[i]->d_name, 255);
            record.size = st.st_size;
            record.mode = st.st_mode;
            security_extract(st.st_mode, &record.security);
            render_row(&record, max_name_len, cfg->use_color);
        }

        free(entries[i]);
    }

    free(entries);
    return 0;
}
