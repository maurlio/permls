# permls

Um utilitário de linha de comando (CLI) minimalista, escrito em C (C11/POSIX), para listagem de diretórios. O permls oferece uma visualização rápida e nativa do sistema de arquivos, destacando permissões de segurança e tamanhos em formato legível.

# Funcionalidades

* **Visualização enxuta:** saída alinhada com separador nativo.
* **Tamanhos legíveis:** conversão automática de bytes para K, M e G.
* **Cores inteligentes:** destaque para diretórios (azul) e executáveis (verde), com detecção automática de TTY e opção de desativar.
* **Arquitetura limpa:** separação entre lógica de permissões, formatação visual e utilitários do sistema.

## Estrutura do Projeto

O projeto segue uma arquitetura modular, facilitando a manutenção e a escalabilidade:

```text
permls/
├── Makefile              # Orquestração de build, testes e análise de memória
├── include/              # Arquivos de cabeçalho públicos (.h)
│   ├── gauge.h           # Interface de renderização
│   ├── perms.h           # Lógica de bits e segurança POSIX
│   └── utils.h           # Utilitários de strings e caminhos
├── src/                  # Implementações (.c)
│   ├── gauge.c           
│   ├── main.c            # Ponto de entrada e injeção de dependências
│   ├── perms.c           
│   └── utils.c           
└── tests/                # Suíte de validação
    ├── integration.sh    # Teste de ponta a ponta (E2E) no bash
    └── unit_tests.c      # Validação matemática e lógica isolada
```

## Compilação

O projeto requer apenas o `gcc` e o utilitário `make`.

1. Clone o repositório:

```bash
git clone https://github.com/maurlio/permls.git
cd permls
```

2. Compile o binário principal:

```bash
make
```

O executável `permls` será gerado na raiz do projeto, e os objetos compilados ficarão isolados na pasta `build/`.

## Uso

Execute o binário passando o diretório alvo ou utilize as flags disponíveis:

```bash
# Lista o diretório atual
./permls

# Lista um diretório específico
./permls --dir /etc

# Desativa as cores (ideal para pipes e redirecionamentos)
./permls --no-color

# Exibe a ajuda
./permls --help
```

## Testes

O projeto conta com uma suíte de testes dividida em unidade e integração. Para rodar todas as validações de uma só vez:

```bash
make test
```

## Licença

Este projeto é distribuído sob os termos da Licença MIT.