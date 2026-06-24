#include "lexer.hpp"

#include <cctype>
#include <sstream>
#include <unordered_map>

const std::string TOKEN_ID = "id";

// Verifica se um caractere é letra.
bool ehLetra(char c) {
    return isalpha(c);
}

// Verifica se um caractere é símbolo da gramática.
bool ehSimbolo(char c) {
    return c == '+' || c == '*' || c == '(' || c == ')';
}


void adicionarToken(std::vector<std::string>& fita, std::vector<RegistroToken>& simbolos, int linha, 
                    const std::string& identificador, const std::string& rotulo) {
    fita.push_back(identificador);
    simbolos.push_back({linha, identificador, rotulo});
}

// Consome uma sequencia continua de caracteres que satisfazem um predicado.

std::string consumirSequencia(const std::string& linha, std::size_t& cursor, bool (*predicado)(char)) {
    std::string resultado;
    while (cursor < linha.size() && predicado(linha[cursor])) {
        resultado += linha[cursor];
        ++cursor;
    }
    return resultado;
}


// Construtor da classe.
AnalisadorLexico::AnalisadorLexico() = default;

// varre a entrada linearmente,
// agrupa símbolos válidos em lexemas,
// classifica cada lexema,
// registra saída estruturada (fita + tabela).
std::pair<std::vector<std::string>, std::vector<RegistroToken>> AnalisadorLexico::analisar(const std::string& fonte) const {
    std::vector<std::string> fita;
    std::vector<RegistroToken> simbolos;
    //cria um stream de string para ler o texto linha por linha
    std::istringstream stream(fonte);
    std::string linha;
    int numeroLinha = 0;
    // coloca cada linha do texto na variavel linha, e processa cada linha
    while (std::getline(stream, linha)) {
        ++numeroLinha;
        // size_t usado na manipulação de índices e tamanhos de strings.
        std::size_t cursor = 0;
        while (cursor < linha.size()) {
            if (std::isspace(linha[cursor])) {
                // Se é ' ' ou '\t' ou '\n', apenas avança o cursor para ignorar.
                ++cursor;
            } else if (ehLetra(linha[cursor])) {
                // Se é letra, consome a sequência de letras para formar um lexema.
                std::string lexema = consumirSequencia(linha, cursor, ehLetra);
                std::string identificador = classificarLexema(lexema);
                adicionarToken(fita, simbolos, numeroLinha, identificador, lexema);
            } else if (ehSimbolo(linha[cursor])) {
                // Se é símbolo, consome o símbolo para formar um lexema.
                std::string lexema(1, linha[cursor]);
                std::string identificador = classificarLexema(lexema);
                adicionarToken(fita, simbolos, numeroLinha, identificador, lexema);
                ++cursor;
            } else {
                // Caractere não é espaço nem letra, classifica como erro
                std::string lexema(1, linha[cursor]);
                adicionarToken(fita, simbolos, numeroLinha, "E99", lexema);
                ++cursor;
            }
        }
    }
    fita.push_back("$");  // Marcador de fim (não remova)
    return {fita, simbolos};
}

std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& tabelaSimbolos) {
    std::string resultado;
    for (const auto& registro : simbolos) {
        tabelaSimbolos[std::to_string(registro.linha)][registro.identificador] = registro.rotulo;
        resultado += std::to_string(registro.linha) + "\t" + registro.identificador + "\t" + registro.rotulo + "\n";
    }
    return resultado;
}

// Classifica um lexema geral.
// - identificadores alfabeticos/alfanumericos -> "id"
// - simbolos da gramatica -> o proprio simbolo
// - qualquer outro padrao -> E99
std::string AnalisadorLexico::classificarLexema(const std::string& lexema) const {
    if (lexema.size() == 1 && ehSimbolo(lexema[0])) {
        return lexema;
    }
    if (lexema.empty()) {
        return "E99";
    }
    for (char c : lexema) {
        if (!std::isalpha(static_cast<unsigned char>(c)) && !std::isdigit(static_cast<unsigned char>(c))) {
            return "E99";
        }
    }
    return TOKEN_ID;
}