#ifndef LEXER_HPP
#define LEXER_HPP

// pair
// Usado no retorno da funcao analisar: {fita, tabela_de_simbolos}.
#include <utility>
// string
// Tipo base para lexemas, estados e identificadores de token.
#include <string>
// vector
// Estrutura sequencial usada para armazenar fita e tabela em ordem de leitura.
#include <vector>

// RegistroToken representa uma linha da Tabela de Simbolos.
// - linha: em qual linha do codigo fonte o lexema apareceu
// - identificador: codigo do token (E1, E2, E99, ...)
// - rotulo: lexema original (ex.: "se", "10", ";")
struct RegistroToken {
    int linha;
    std::string identificador;
    std::string rotulo;
};

class AnalisadorLexico {
public:
    // Construtor padrao.
    // Nesta versao, as tabelas estao em constantes no .cpp,
    // entao o construtor nao precisa inicializar estado interno complexo.
    AnalisadorLexico();

    // Analisa todo o texto de entrada e retorna:
    // first  -> FITA de tokens, finalizada com "$"
    // second -> Tabela de Simbolos
    std::pair<std::vector<std::string>, std::vector<RegistroToken>> analisar(const std::string& fonte) const;

private:
    // Classifica uma palavra (apenas letras) percorrendo o AFD.
    // Ex.: "se" -> E1, "sai" -> E2, "foi" -> E3, "eaf" -> E4.
    std::string classificarPalavraComAFD(const std::string& lexema) const;

    // Classifica um lexema geral:
    // - somente digitos -> E5
    // - somente letras  -> classificarPalavraComAFD
    // - mistura/invalid -> E99
    std::string classificarLexema(const std::string& lexema) const;
};

// Gera texto tabulado da tabela de simbolos para impressao e arquivo.
std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos);

#endif
