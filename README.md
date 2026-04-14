# Reconhecedor lexico - Projeto 1 (C++)

A descricao do trabalho pode ser encontrada em `DescricaoTrabalhoAnalisadorLexico.pdf`.

## O que este starter implementa

- Definicao inicial de tokens:
	- `E1`: palavra reservada `se`
	- `E2`: palavra reservada `sai`
	- `E3`: palavra reservada `foi`
	- `E4`: identificador pela GR `[faei]+`
	- `E99`: erro lexico
- AFD determinizado (baseado no exemplo do enunciado) com estado de erro `X`
- Algoritmo de reconhecimento que gera:
	- FITA no formato `E1 E2 ... $`
	- Tabela de simbolos com `linha`, `identificador`, `rotulo`

## Estrutura

- `main.cpp`: ponto de execucao
- `lexer.hpp`: declaracoes do analisador
- `lexer.cpp`: implementacao do analisador lexico e tabela AFD
- `input.txt`: entrada de exemplo
- `output/fita.txt`: fita gerada
- `output/tabela_simbolos.tsv`: tabela de simbolos gerada

## Como compilar e executar

Com `g++` (C++17):

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp lexer.cpp -o lexer
./lexer
```

No Windows (PowerShell), execute:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp lexer.cpp -o lexer.exe
.\lexer.exe
```

## Proximos passos sugeridos

1. Expandir conjunto de tokens (outros simbolos, operadores e constantes).
2. Implementar AFND e rotina de determinizacao para os novos tokens.
3. Melhorar tratamento de erros (mensagem, coluna e recuperacao).
4. Integrar com parser sintatico nas proximas etapas da disciplina.
