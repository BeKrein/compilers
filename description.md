UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
CIÊNCIA DA COMPUTAÇÃO – Campus Chapecó
Disciplina de Construção de Compiladores
Descrição do PROJETO 1: Construção do Analisador Léxico
Braulio Mello
Última atualização: 19/03/26
Projeto 1: Reconhecedor léxico
Objetivo:
Construção de um analisador léxico para reconhecimento de tokens de uma linguagem.
Requisitos:

1. Definição dos tokens da linguagem (palavras reservadas, identificadores, símbolos especiais,
   constantes).
2. Construção do AFND e determinização (pode ser implementado ou feito apenas para um
   conjunto de tokens)
3. Implementação do algoritmo de mapeamento do AFD para reconhecimento léxico
4. Geração da fita de saída (FITA: E1 E2 E3... $) onde En é o identificador de cada token
   reconhecido.
5. Geração da Tabela de Símbolos com, no mínimo, as seguintes informações da cadeia/sentença:
   linha, identificador, rótulo
   Passos:

- Definir conjunto de tokens
- Construir AFND e determinizar
- Adicionar estado de erro no AFD
- Implementar algoritmo de reconhecimento (reconhecimento, gera TS e FITA)
  • separadores
  • tratamento de erro
  • gera FITA de saída
  • gera informações na Tabela de Símbolos
  Exemplo:
  Para os seguintes tokens
  se
  sai
  foi
  e a GR (gramática regular) que define cadeias para rótulo de variáveis:
  S::= fA | aA | eA | iA
  A::= fA | aA | eA | iA |
  e
  O ANFD é:
  s e a i f o
  ->S A,C I I I F,I
  A B
  *B
  C D
  D E
  *E
  F G
  G H
  *H
  *I I I I I
  Determinizando e adicionando estado de erro temos:
  s e a i f o @
  ->S [AC] I I I [FI] x x
  [AC] x B D x x x x
  _I x I I I I x x
  _[FI] x I I I I G x
  *B x x x x x x x
  D x x x E x x x
  G x x x H x G x
  *E x x x H x x x
  *H x x x x x x x
  *X x x x x x x x
  Algoritmo de reconhecimento:
  1 EC = S
  2 Ler(Sb)
  3 se separador vai para 6
  4 EC = AF[EC,Sb] (ocorre transição)
  5 vai para 2
  6 se estado não final EC = X
  7 add FITA(EC)
  8 Add TS(linha, EC, label)
  9 vai para 1
  Texto:
  Elaborar artigo entre 2 e 4 páginas contendo:
  • Título, autores e instituição
  • Introdução: Contextualização sobre reconhecedores léxicos (propósitos, aplicação e
  características), descrição breve do conteúdo do artigo. (até 1/2 página)
  • Referencial teórico: Breve explanação sobre os conceitos, técnicas e/ou teoremas fundamentais
  para o desenvolvimento do trabalho. (até 1 página)
  • Implementação e resultados: Apresentação dos detalhes de especificação do analisador léxico
  (tokens, decisões de projeto, GLC, etc), implementação e validação. (até 1,5 página)
  • Conclusões re referências: O que foi feito, dificuldades, resultados finais e perspectivas para
  continuidade do trabalho (por exemplo, sugerir alterações futuras para utilizar a implementação
  no ensino de compiladores). (até 1/2 página)
  Construir o texto na forma de artigo (seções corridas), não de relatório (capa, etc).
  Apresentação:
  • Trabalho individual ou em dupla
  • A apresentação será na modalidade pergunta (docente) e resposta (discente). Não é necessário
  preparar apresentação (slides, etc).
  • A nota será de acordo com o domínio dos detalhes do projeto prático demonstrado através
  arguição.
  • A não apresentação do projeto resultará em nota 0 (zero), independente da entrega da
  implementação no SIGAA.
  • Datas e prazos no plano de curso da disciplina.
