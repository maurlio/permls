#!/bin/bash

# Cores para o output do teste
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "Executando Testes de Integração"

# 1. Verifica se o binário existe
if [ ! -f "./permls" ]; then
    echo -e "${RED}[FAIL] Binário permls não encontrado. Execute 'make' primeiro.${NC}"
    exit 1
fi

# 2. Teste de execução básica
./permls --dir . > /dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[PASS] Execução básica (exit code 0)${NC}"
else
    echo -e "${RED}[FAIL] O programa falhou ao listar o diretório atual${NC}"
    exit 1
fi

# 3. Verifica se o formato contém o separador pipe '|'
output=$(./permls --dir . | head -n 5)
if [[ $output == *"|"* ]]; then
    echo -e "${GREEN}[PASS] Formato visual (presença do separador pipe)${NC}"
else
    echo -e "${RED}[FAIL] O formato da saída está incorreto${NC}"
    exit 1
fi

echo "Todos os testes de integração passaram!"