#include "lexer.hpp"

#include <cctype>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

const std::string ESTADO_INICIAL = "S";

const std::string ESTADO_ERRO = "X";

const std::unordered_set<char> ALFABETO = {'s', 'e', 'a', 'i', 'f', 'o'};

// unordered_map: estrutura de dados de mapeamento chave-valor, usada para o AFD e estados finais.
// nesse caso são 2 mapas: um para o AFD (mapa de mapas) e outro para os estados finais (mapa simples).
const std::unordered_map<std::string, std::unordered_map<char, std::string>> AFD = {
    {"S",  {{'s', "AC"}, {'e', "I"}, {'a', "I"}, {'i', "I"}, {'f', "FI"}, {'o', "X"}, {'@', "X"}}},
    {"AC", {{'s', "X"},  {'e', "B"}, {'a', "D"}, {'i', "X"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}},
    {"I",  {{'s', "X"},  {'e', "I"}, {'a', "I"}, {'i', "I"}, {'f', "I"},  {'o', "X"}, {'@', "X"}}},
    {"FI", {{'s', "X"},  {'e', "I"}, {'a', "I"}, {'i', "I"}, {'f', "I"},  {'o', "G"}, {'@', "X"}}},
    {"B",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "X"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}},
    {"D",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "E"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}},
    {"G",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "H"}, {'f', "X"},  {'o', "G"}, {'@', "X"}}},
    {"E",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "H"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}},
    {"H",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "X"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}},
    {"X",  {{'s', "X"},  {'e', "X"}, {'a', "X"}, {'i', "X"}, {'f', "X"},  {'o', "X"}, {'@', "X"}}}
};

// Mapeia estados finais do AFD para identificadores de token.
const std::unordered_map<std::string, std::string> ESTADOS_FINAIS = {
    {"B", "E1"},
    {"E", "E2"},
    {"H", "E3"},
    {"I", "E4"}
};


// Verifica se um caractere é letra.
bool ehLetra(char c) {
    return isalpha(c);
}

// Adiciona um token na fita e na tabela de simbolos ao mesmo tempo.
// Exemplo de uso: adicionarToken(fita, simbolos, 2, "E1", "se");
// recebe: linha = 2, identificador = "E1", rotulo = "se"
// resultado: fita recebe "E1", simbolos recebe {2, "E1", "se"}
void adicionarToken(std::vector<std::string>& fita, std::vector<RegistroToken>& simbolos, int linha, 
                    const std::string& identificador, const std::string& rotulo) {
    fita.push_back(identificador);
    simbolos.push_back({linha, identificador, rotulo});
}

// Consome uma sequencia continua de caracteres que satisfazem um predicado.
// exemplo de uso: consumirSequencia(linha, 3, ehLetra).
// recebe: linha = "sai se foi"
// retorna: "se"
// atualiza o cursos para a posicao logo apos a sequencia consumida (cursor = 5, apontando para o espaco).
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

// Classifica uma palavra percorrendo o AFD.
// exemplo de uso: classificarPalavraComAFD("se")
// recebe: "se"
// retorna: "E1" (estado final B do AFD)

std::string AnalisadorLexico::classificarPalavraComAFD(const std::string& lexema) const {
    std::string estadoAtual = ESTADO_INICIAL;
    for (char c : lexema){
        // count() retorna 0 se o caractere não estiver no alfabeto, ou 1 se estiver.
        if (ALFABETO.count(c) == 0) {
            return "X";  // Caractere fora do alfabeto leva ao estado de erro
        }
                      //Acha no AFD o próximo estado a partir do estado atual e do caractere lido
        estadoAtual = AFD.at(estadoAtual).at(c);
    }
    // Verifica se o estado atual é final e retorna o identificador correspondente, ou E99 se não for final
    if (ESTADOS_FINAIS.count(estadoAtual) > 0) {
        return ESTADOS_FINAIS.at(estadoAtual);
    }
    return "E99";  // Estado não final, erro
}

// Classifica um lexema geral.
// Caso não implementar numeros deletar essa função
// Se for apenas letras: chama classificarPalavraComAFD
// Senão: retorna E99
// std::string AnalisadorLexico::classificarLexema(const std::string& lexema) const {
// }

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
                std::string identificador = classificarPalavraComAFD(lexema);
                adicionarToken(fita, simbolos, numeroLinha, identificador, lexema);
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

std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos) {
    std::string resultado;
    for (const auto& registro : simbolos) {
        resultado += std::to_string(registro.linha) + "\t" + registro.identificador + "\t" + registro.rotulo + "\n";
    }
    return resultado;
}
