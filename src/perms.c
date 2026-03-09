#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "perms.h"

void perms_extract(const struct stat *st, FilePermissions *fp)
{
    mode_t mode = st->st_mode;

    fp->owner_read = mode & S_IRUSR;
    fp->owner_write = mode & S_IWUSR;
    fp->owner_execute = mode & S_IXUSR;

    fp->group_read = mode & S_IRGRP;
    fp->group_write = mode & S_IWGRP;
    fp->group_execute = mode & S_IXGRP;

    fp->other_read = mode & S_IROTH;
    fp->other_write = mode & S_IWOTH;
    fp->other_execute = mode & S_IXOTH;
}

const char *perms_get_file_type(mode_t mode)
{
    if (S_ISDIR(mode))
        return "dir";

    if (S_ISLNK(mode))
        return "link";

    return "arq";
}

void perms_format_full(char *dest, size_t size, bool r, bool w, bool x)
{
    if (!dest || size == 0)
        return;

    int n = 0;

    if (r)
        n += snprintf(dest + n, size - n, "%s", PERM_READ);
    if (w)
        n += snprintf(dest + n, size - n, "%s%s", (n > 0) ? ", " : "", PERM_WRITE);
    if (x)
        n += snprintf(dest + n, size - n, "%s%s", (n > 0) ? ", " : "", PERM_EXECUTE);
    if (n == 0)
        snprintf(dest, size, "%s", PERM_NONE);
}

void perms_format_compact(char *dest, size_t size, bool r, bool w, bool x)
{
    snprintf(dest, size, "[%s][%s][%s]", r ? "l" : "-", w ? "e" : "-", x ? "x" : "-");
}