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

## ⚙️ Como Compilar e Executar

Para compilar o projeto garantindo que todas as dependências e módulos sejam vinculados corretamente, execute o comando abaixo no terminal:

```bash
gcc -Wall -Wextra -std=c99 main.c produtos.c estruturas.c -o programa
```

Em seguida, execute a aplicação gerada:

* **Linux / macOS:**
  ```bash
  ./programa
  ```
* **Windows (CMD / PowerShell):**
  ```powershell
  .\programa.exe
  ```


---

##  Licença
Projeto desenvolvido para fins acadêmicos.


---

