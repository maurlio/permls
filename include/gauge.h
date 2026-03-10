#ifndef GAUGE_H
#define GAUGE_H

#include "utils.h"
#include "perms.h"

typedef struct
{
    char name[256];
    long long size;
    mode_t mode;
    FileSecurity security;
} FileRecord;

/**
 * Inicia o ciclo de listagem e renderização do diretório
 */
int core_list_directory(const Config *cfg);

#endif