#ifndef GAUGE_H
#define GAUGE_H

#include <sys/types.h>
#include "utils.h"
#include "perms.h"

/**
 * @brief Exibe as informações de um arquivo formatadas de acordo com as opções fornecidas.
 * @param meta Metadados do arquivo a ser exibido.
 * @param opts Opções para determinar o modo de exibição e outras configurações.
 * @param mode Modo do arquivo (obtido de stat) para determinar o tipo e as permissões.
 * @param name_width Largura da coluna de nomes.
 */
void display_file_info(const FileMeta *meta, const Options *opts, mode_t mode, int width, const char *parent_dir);

/**
 * @brief Lista o conteúdo de um diretório e exibe as informações formatadas.
 * @param directory Caminho do diretório a ser listado.
 * @param opts Opções para determinar o modo de exibição e outras configurações.
 */
int list_directory_contents(const char *directory, const Options *opts);

#endif