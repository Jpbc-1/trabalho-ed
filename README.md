# Sistema de Gerenciamento de Produtos - Estrutura de Dados

Projeto prático desenvolvido em C para a disciplina de Estruturas de Dados, focado na manipulação, organização e processamento eficiente de registros de produtos a partir de arquivos de entrada.

---

##  Estrutura do Projeto

```text
.
├── main.c           # Ponto de entrada da aplicação e fluxo principal
├── produtos.c       # Implementação das regras de negócio e manipulação dos produtos
├── produtos.h       # Protótipos e interfaces das funções de produtos
├── estruturas.c     # Implementação das estruturas de dados utilizadas
├── estruturas.h     # Definições de tipos e estruturas de dados
├── entrada.txt      # Arquivo de dados de teste/exemplo
├── entrada2.txt     # Arquivo complementar para testes de carga/casos de borda
└── README.md

```
---

## Tecnologias & Pré-requisitos

- Linguagem: C (padrão C99 ou superior)

- Compilador: gcc (ou clang)

- Terminal/Shell: Linux, macOS ou Windows (via WSL / MinGW / Git Bash)

---

## Como Compilar e Executar
1. Compilação
No terminal, dentro do diretório do projeto, execute:

Bash
gcc -Wall -Wextra -std=c99 main.c produtos.c estruturas.c -o programa

2. Execução
Execute o binário gerado:

Linux / macOS:

Bash
./programa
Windows:

DOS
programa.exe
Caso o programa espere o nome do arquivo de entrada via parâmetro:

Bash
./programa entrada.txt

---

##  Licença
Projeto desenvolvido para fins acadêmicos.


---

