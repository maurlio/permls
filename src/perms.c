#include "perms.h"

void security_extract(mode_t mode, FileSecurity *security)
{
    security->user = (AccessLevel){mode & S_IRUSR, mode & S_IWUSR, mode & S_IXUSR};
    security->group = (AccessLevel){mode & S_IRGRP, mode & S_IWGRP, mode & S_IXGRP};
    security->other = (AccessLevel){mode & S_IROTH, mode & S_IWOTH, mode & S_IXOTH};
}

const char *security_get_type_label(mode_t mode)
{
    if (S_ISDIR(mode))
        return "dir";

    if (S_ISLNK(mode))
        return "link";

    return "arq";
}

void security_format_triplet(char *dest, AccessLevel level)
{
    dest[0] = level.read ? 'r' : '-';
    dest[1] = level.write ? 'w' : '-';
    dest[2] = level.execute ? 'x' : '-';
    dest[3] = '\0';
}
