/**
 * @file gauge.h
 * @brief Protótipos para listagem, coleta de metadados e apresentação final.
 */

#ifndef GAUGE_H
#define GAUGE_H

// Inclui dependências de módulos
#include "utils.h"
#include "perms.h"

// PROTÓTIPO DE FUNÇÃO

/**
 * @brief Inicia a listagem e exibição do conteúdo de um diretório.
 *
 * Usa opendir/readdir e stat/lstat.
 *
 * @param directory Caminho do diretório.
 * @param opts Ponteiro para as opções de execução.
 * @return 0 em sucesso, ou -1 em erro (ex: falha ao abrir dir).
 */
int list_directory_contents(const char *directory, const Options *opts);

/**
 * @brief Exibe as informações e permissões formatadas de um único arquivo.
 *
 * @param meta Estrutura de metadados e permissões.
 * @param opts Opções para determinar o modo de exibição (--full ou --compact).
 */
void display_file_info(const FileMeta *meta, const Options *opts);

#endif // GAUGE_H