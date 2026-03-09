# permls

Ferramenta de linha de comando em **C** para visualizar permissões de arquivos no Linux de forma clara.

O `permls` traduz permissões `rwx` para **ler**, **escrever** e **executar**, exibindo os resultados em uma **tabela simples e alinhada**.

## Requisitos

* `gcc`
* `make`

Instalar dependências (Ubuntu/Debian):

```bash
sudo apt install build-essential
```

## Compilação

```bash
make
```

Limpar arquivos gerados:

```bash
make clean
```

## Uso

Executar no diretório atual:

```bash
./permls
```

Listar outro diretório:

```bash
./permls --dir /etc
```

## Opções

| Opção             | Descrição                                        |
| ----------------- | ------------------------------------------------ |
| `--dir <caminho>` | Diretório a ser listado                          |
| `--full`          | Permissões completas (`ler, escrever, executar`) |
| `--compact`       | Permissões curtas (`L, W, X`)                    |
| `--meta`          | Exibe metadados adicionais                       |
| `--help`          | Mostra ajuda                                     |

## Exemplo de saída

```
tipo  nome                 dono                         grupo                        outros
------------------------------------------------------------------------------------------------
dir   src                  ler, escrever, executar      ler, executar                ler, executar
arq   README.md            ler, escrever                ler                          ler
```

## Implementação

* Linguagem: **C**
* APIs POSIX: `opendir()`, `readdir()`, `stat()`

## Licença

Este projeto é distribuído sob os termos da Licença MIT.