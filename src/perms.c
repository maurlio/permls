#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "perms.h"

void perms_extract(const struct stat *st, FilePermissions *fp)
{
    mode_t mode = st->st_mode;
    fp->owner_read = (mode & S_IRUSR);
    fp->owner_write = (mode & S_IWUSR);
    fp->owner_execute = (mode & S_IXUSR);
    fp->group_read = (mode & S_IRGRP);
    fp->group_write = (mode & S_IWGRP);
    fp->group_execute = (mode & S_IXGRP);
    fp->other_read = (mode & S_IROTH);
    fp->other_write = (mode & S_IWOTH);
    fp->other_execute = (mode & S_IXOTH);
}

const char *perms_get_file_type(mode_t mode)
{
    if (S_ISDIR(mode))
        return "diretório";
    if (S_ISLNK(mode))
        return "link simbólico";
    if (S_ISCHR(mode))
        return "dispositivo de caractere";
    if (S_ISBLK(mode))
        return "dispositivo de bloco";
    if (S_ISFIFO(mode))
        return "fifo/pipe";
    if (S_ISSOCK(mode))
        return "socket";
    return "arquivo";
}

void perms_format_full(char *dest, size_t size, bool read, bool write, bool execute)
{
    if (!dest || size == 0)
        return;

    int offset = 0;
    if (read)
    {
        offset += snprintf(dest + offset, size - offset, "%s", PERM_READ);
    }
    if (write)
    {
        if (offset > 0)
            offset += snprintf(dest + offset, size - offset, ", ");
        offset += snprintf(dest + offset, size - offset, "%s", PERM_WRITE);
    }
    if (execute)
    {
        if (offset > 0)
            offset += snprintf(dest + offset, size - offset, ", ");
        offset += snprintf(dest + offset, size - offset, "%s", PERM_EXECUTE);
    }

    if (offset == 0)
    {
        snprintf(dest, size, "%s", PERM_NONE);
    }
}

void perms_format_compact(char *dest, size_t size, bool read, bool write, bool execute)
{
    snprintf(dest, size, "[%s][%s][%s]",
             read ? "l" : PERM_NONE,
             write ? "e" : PERM_NONE,
             execute ? "x" : PERM_NONE);
}