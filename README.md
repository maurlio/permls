# Permls: Permissões de Arquivos Simplificadas para Iniciantes

![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-yellow)

O **permls** é uma ferramenta de linha de comando desenvolvida em C para simplificar a visualização de permissões de arquivos em sistemas Linux.

Seu objetivo é **traduzir** as permissões tradicionais (`rwx`) para uma linguagem clara e acessível — **ler**, **escrever**, **executar** — exibindo-as em um **formato de tabela alinhada e minimalista**, ideal para iniciantes e para quem busca uma leitura rápida.

## Funcionalidades Principais

* **Tradução Clara:** Converte `r`, `w`, `x` para palavras em Português (`ler`, `escrever`, `executar`).
* **Design Minimalista:** Saída formatada como uma tabela rígida e perfeitamente alinhada, eliminando ruído visual (como prefixos `OWNER:`, etc.).
* **Abreviações:** Tipos de arquivo abreviados para `dir` e `arq` para máxima economia de espaço.
* **Seletor de Diretório:** Use `--dir` para listar qualquer pasta do sistema.

## Tecnologias e Recursos

- **Linguagem:** C  
- **APIs POSIX:** `opendir()`, `readdir()`, `stat()`  
- **Conceito Central:** Manipulação de bits de permissão em `st_mode`

## Instalação e Compilação

O projeto utiliza um **Makefile** para facilitar o processo.

### 1. Requisitos

É necessário ter `gcc` e `make` instalados (padrão em distribuições Linux como Ubuntu 24.04 LTS):

```bash
sudo apt update
sudo apt install build-essential
```

### 2. Compilação

No diretório raiz do projeto (`permls/`):

```bash
make
```

O executável `permls` será criado diretamente na raiz do projeto.

### 3. Limpeza

Para remover os arquivos objeto e o executável:

```bash
make clean
```

## Uso

O `permls` pode ser executado sem argumentos (padrão: diretório atual) ou com parâmetros específicos.

### Argumentos da Linha de Comando

| Parâmetro          | Descrição                                                   | Padrão                   |
|-------------------|---------------------------------------------------------------|--------------------------|
| `--dir <caminho>` | Especifica o diretório a ser listado                         | Diretório atual (`.`)    |
| `--full`          | Exibe permissões por extenso (LER, ESCREVER, EXECUTAR)       | Ativo se `--compact` não for usado |
| `--compact`       | Modo enxuto (L, W, X)                                        | Desativado               |
| `--meta`          | Inclui metadados extras (tamanho, inode, tipo)               | Desativado               |
| `--help`          | Exibe ajuda                                                   | —                        |

### Exemplos de Uso

**1. Modo Padrão (Tabela Alinhada):**

```bash
./permls
```

**2. Listar o diretório `/etc` no modo compacto:**

```bash
./permls --dir /etc --compact
```

**3. Listar com Metadados:**

```bash
./permls --meta
```

**4. Saída Esperada (Modo Padrão):**

```
tipo  nome                 dono                         grupo                        outros
------------------------------------------------------------------------------------------------------
dir   .git                 ler, escrever, executar      ler, escrever, executar      ler, executar
arq   README.md            ler, escrever                ler                          ler
dir   src                  ler, escrever, executar      ler, executar                ler, executar
```

## Aviso de Segurança

O `permls` é **somente leitura**.  
Ele **não altera**, **não exclui** e **não modifica** permissões de arquivos. Apenas interpreta e exibe informações.

## Licença

Este projeto está sob a licença **MIT**.

