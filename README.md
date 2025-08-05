# Minishell

Uma implementação simplificada de um shell Unix em C, desenvolvida como projeto da 42 School.

## 📋 Índice

- [Sobre o Projeto](#sobre-o-projeto)
- [Funcionalidades](#funcionalidades)
- [Instalação](#instalação)
- [Uso](#uso)
- [Comandos Internos (Builtins)](#comandos-internos-builtins)
- [Redirecionamentos](#redirecionamentos)
- [Pipes](#pipes)
- [Expansão de Variáveis](#expansão-de-variáveis)
- [Tratamento de Sinais](#tratamento-de-sinais)
- [Exemplos](#exemplos)
- [Arquitetura](#arquitetura)
- [Contribuições](#contribuições)

## 🎯 Sobre o Projeto

O **Minishell** é uma implementação de um interpretador de linha de comando (shell) que replica as funcionalidades básicas do bash. Este projeto faz parte do currículo da 42 School e tem como objetivo aprofundar o conhecimento sobre processos, redirecionamentos, pipes e manipulação de strings em C.

### Características Principais

- Interface interativa com histórico de comandos (usando readline)
- Tokenização e parsing de comandos
- Execução de comandos externos via PATH
- Sistema de pipes para comunicação entre processos
- Redirecionamentos de entrada e saída
- Expansão de variáveis de ambiente
- Suporte a heredoc
- Tratamento adequado de sinais

## ⚡ Funcionalidades

### ✅ Implementadas

- **Prompt interativo** com histórico de comandos
- **Execução de comandos externos** encontrados no PATH
- **Comandos internos (builtins)**:
  - `echo` com opção `-n`
  - `cd` com caminhos relativos e absolutos
  - `pwd` (print working directory)
  - `export` para definir variáveis de ambiente
  - `unset` para remover variáveis de ambiente
  - `env` para listar variáveis de ambiente
  - `exit` com códigos de saída
- **Redirecionamentos**:
  - `<` (input)
  - `>` (output)
  - `>>` (append)
  - `<<` (heredoc)
- **Pipes** (`|`) para conectar comandos
- **Expansão de variáveis** (`$VAR`, `$?`)
- **Aspas simples e duplas** para controle de expansão
- **Tratamento de sinais** (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

## 🚀 Instalação

### Pré-requisitos

- GCC ou outro compilador C
- Make
- Biblioteca readline-dev

```bash
# Ubuntu/Debian
sudo apt-get install libreadline-dev

# MacOS (com Homebrew)
brew install readline
```

### Compilação

```bash
# Clone o repositório
git clone <repository-url>
cd minishell

# Compile o projeto
make

# Execute o minishell
./minishell
```

### Comandos Make Disponíveis

```bash
make        # Compila o projeto
make clean  # Remove arquivos objeto
make fclean # Remove executável e arquivos objeto
make re     # Recompila completamente
```

## 🖥️ Uso

Após compilar, execute o minishell:

```bash
./minishell
```

Você verá um prompt colorido aguardando comandos:

```
minishell$
```

## 🔧 Comandos Internos (Builtins)

### `echo`
Imprime argumentos na saída padrão.
```bash
minishell$ echo "Hello World"
Hello World
minishell$ echo -n "No newline"
No newline$
```

### `cd`
Altera o diretório atual.
```bash
minishell$ cd /home/user
minishell$ cd ..
minishell$ cd ~
minishell$ cd -    # Volta ao diretório anterior
```

### `pwd`
Mostra o diretório atual.
```bash
minishell$ pwd
/home/user/minishell
```

### `export`
Define ou modifica variáveis de ambiente.
```bash
minishell$ export VAR=value
minishell$ export PATH="/usr/bin:$PATH"
minishell$ export          # Lista todas as variáveis
```

### `unset`
Remove variáveis de ambiente.
```bash
minishell$ unset VAR
minishell$ unset PATH HOME  # Remove múltiplas variáveis
```

### `env`
Lista todas as variáveis de ambiente.
```bash
minishell$ env
PATH=/usr/bin:/bin
HOME=/home/user
USER=user
...
```

### `exit`
Sai do minishell.
```bash
minishell$ exit 0    # Sai com código 0
minishell$ exit      # Sai com último status
```

## 📁 Redirecionamentos

### Redirecionamento de Entrada (`<`)
```bash
minishell$ cat < file.txt
minishell$ grep "pattern" < input.txt
```

### Redirecionamento de Saída (`>`)
```bash
minishell$ echo "Hello" > output.txt
minishell$ ls > file_list.txt
```

### Redirecionamento de Saída (Append) (`>>`)
```bash
minishell$ echo "New line" >> output.txt
minishell$ date >> log.txt
```

### Heredoc (`<<`)
```bash
minishell$ cat << EOF
> Line 1
> Line 2
> EOF
Line 1
Line 2
```

## 🔗 Pipes

Conecta a saída de um comando à entrada do próximo:

```bash
minishell$ ls | grep ".c"
minishell$ cat file.txt | grep "pattern" | wc -l
minishell$ ps aux | grep minishell | awk '{print $2}'
```

## 🔄 Expansão de Variáveis

### Variáveis de Ambiente
```bash
minishell$ echo $HOME
/home/user
minishell$ echo $PATH
/usr/bin:/bin:/usr/local/bin
```

### Status do Último Comando (`$?`)
```bash
minishell$ ls
...
minishell$ echo $?
0
minishell$ ls /nonexistent
ls: cannot access '/nonexistent': No such file or directory
minishell$ echo $?
2
```

### Aspas e Expansão
```bash
minishell$ VAR="Hello World"
minishell$ echo "$VAR"        # Expande
Hello World
minishell$ echo '$VAR'        # Não expande
$VAR
```

## ⚡ Tratamento de Sinais

- **Ctrl+C (SIGINT)**: Interrompe o comando atual e mostra novo prompt
- **Ctrl+D (EOF)**: Sai do minishell quando não há input
- **Ctrl+\ (SIGQUIT)**: Ignorado no modo interativo

## 📝 Exemplos

### Exemplos Básicos
```bash
# Comando simples
minishell$ ls -la

# Comando com pipe
minishell$ ps aux | grep bash

# Redirecionamento
minishell$ echo "test" > file.txt
minishell$ cat < file.txt

# Múltiplos pipes
minishell$ cat /etc/passwd | grep user | cut -d: -f1
```

### Exemplos Avançados
```bash
# Combinando redirecionamentos e pipes
minishell$ ls | grep ".c" > c_files.txt

# Usando variáveis
minishell$ export GREETING="Hello"
minishell$ echo "$GREETING World" | cat > greeting.txt

# Heredoc com pipes
minishell$ cat << EOF | grep "important"
> This is important
> This is not
> EOF
This is important
```

## 🏗️ Arquitetura

O minishell é estruturado em várias componentes principais:

### 1. **Tokenizer** (`srcs/tokenizer/`)
- Quebra a linha de comando em tokens
- Identifica tipos de tokens (palavras, pipes, redirecionamentos)
- Implementa máquina de estados para parsing

### 2. **Parser** (`srcs/parsing_tree/`)
- Constrói árvore sintática abstrata (AST)
- Organiza tokens em estrutura hierárquica
- Identifica precedência de operadores

### 3. **Expander** (`srcs/expander/`)
- Expande variáveis de ambiente
- Processa aspas simples e duplas
- Remove caracteres de escape

### 4. **Executor** (`srcs/executor/`)
- Executa comandos baseado na AST
- Gerencia processos filhos
- Implementa pipes e redirecionamentos

### 5. **Builtins** (`srcs/builtin/`)
- Implementação de comandos internos
- Manipulação de variáveis de ambiente
- Controle de diretório e processo

### 6. **Gestão de Ambiente** (`srcs/env/`)
- Lista ligada de variáveis de ambiente
- Operações de busca, inserção e remoção
- Conversão entre formatos

### Estruturas de Dados Principais

```c
// Token individual
typedef struct s_token {
    int     type;           // Tipo do token
    char    *lexeme;        // Texto do token
} t_token;

// Nó da árvore sintática
typedef struct s_tree {
    int             tkn_type;   // Tipo do nó
    t_list          *sub_list;  // Lista de tokens
    t_redir         *redir;     // Redirecionamentos
    int             fd[2];      // File descriptors para pipes
    struct s_tree   *left;      // Filho esquerdo
    struct s_tree   *right;     // Filho direito
    struct s_tree   *parent;    // Nó pai
} t_tree;

// Variável de ambiente
typedef struct s_envp {
    char            *key;       // Nome da variável
    char            *value;     // Valor da variável
    struct s_envp   *next;      // Próximo nó
} t_envp;
```

## 🤝 Contribuições

Este é um projeto acadêmico da 42 School. Sugestões e melhorias são bem-vindas através de issues e pull requests.

### Desenvolvedores
- **cshingai** - Desenvolvimento principal
- **lsouza-r** - Desenvolvimento principal

---

**Nota**: Este projeto foi desenvolvido seguindo as normas da 42 School e tem propósitos educacionais. Para uso em produção, considere shells mais robustos como bash ou zsh.
