#ifndef LEXER_HPP
#define LEXER_HPP

//para o std::pair
#include <utility>

// para ultilizar strings
#include <string>

// para ultilizar vector
#include <vector>

// para ultilizar unordered_map na assinatura de formatarTabelaSimbolos
#include <unordered_map>

// RegistroToken representa uma linha da Tabela de Simbolos.
// - linha: em qual linha do codigo fonte o lexema apareceu
// - identificador: codigo do token (id, +, *, (, ), E99)
// - rotulo: lexema original (ex.: "x", "+", "(")
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
    // Classifica um lexema geral:
    // - identificador -> "id"
    // - simbolos da gramatica -> o proprio simbolo
    // - qualquer outro padrao -> E99
    std::string classificarLexema(const std::string& lexema) const;
};

// Gera texto tabulado da tabela de simbolos para impressao e arquivo.
std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& tabelaSimbolos);

#endif
