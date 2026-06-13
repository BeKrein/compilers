#ifndef LEXER_HPP
#define LEXER_HPP

//para o std::pair
#include <utility>

// para ultilizar strings
#include <string>

// para ultilizar vector
#include <vector>

// RegistroToken representa uma linha da Tabela de Simbolos.
// - linha: em qual linha do codigo fonte o lexema apareceu
// - identificador: codigo do token (E1, E2, E3, E4, E99)
// - rotulo: lexema original (ex.: "se", "sai", "foi")
struct RegistroToken {
    int linha;
    std::string identificador;
    std::string rotulo;
};

class AnalisadorLexico {
public:
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
    // - somente letras -> classificarPalavraComAFD
    // - qualquer outro padrao -> E99
    std::string classificarLexema(const std::string& lexema) const;
};

// Gera texto tabulado da tabela de simbolos para impressao e arquivo.
std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& tabelaSimbolos);

#endif
