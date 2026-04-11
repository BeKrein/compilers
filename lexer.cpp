#include "lexer.hpp"

#include <cctype>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace {
// ==========================
// GUIA RAPIDO: unordered_map
// ==========================
// std::unordered_map<K, V> guarda pares (chave -> valor) usando tabela hash.
// Caracteristicas praticas:
// - Busca/insercao tipicamente O(1) medio.
// - Nao preserva ordem de insercao.
// - ideal para "dado uma chave, qual o valor associado?"
//
// Operacoes usadas neste projeto:
// - mapa.find(chave):
//   retorna iterador para o par encontrado, ou mapa.end() se nao existir.
// - mapa.count(chave):
//   retorna 1 se existe, 0 se nao existe.
//
// Exemplo simples:
// unordered_map<char, string> m = {{'a', "E1"}};
// auto it = m.find('a');
// if (it != m.end()) {
//     // it->first  = 'a'
//     // it->second = "E1"
// }

// Estado inicial do automato.
const std::string ESTADO_INICIAL = "S";
// Estado de erro/sumidouro do automato.
const std::string ESTADO_ERRO = "X";

// Alfabeto valido para as palavras modeladas pelo AFD do enunciado.
// unordered_set tambem e hash-based e bom para "pertence / nao pertence".
const std::unordered_set<char> ALFABETO = {'s', 'e', 'a', 'i', 'f', 'o'};

// AFD determinizado:
// - Chave externa: estado atual (string)
// - Valor externo: mapa de transicoes desse estado
// - Chave interna: simbolo lido (char)
// - Valor interno: proximo estado (string)
//
// Leitura semantica:
// AFD["S"]['s'] == "AC"
// significa: no estado S, lendo 's', vai para estado AC.
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
// Se um estado nao estiver aqui, ele nao e considerado final para o lexer.
const std::unordered_map<std::string, std::string> ESTADOS_FINAIS = {
    {"B", "E1"},
    {"E", "E2"},
    {"H", "E3"},
    {"I", "E4"}
};

// Mapeia simbolos especiais para token id.
const std::unordered_map<char, std::string> SIMBOLOS_ESPECIAIS = {
    {'=', "E6"},
    {';', "E7"}
};

// Wrappers pequenos para deixar o codigo principal mais legivel.
bool ehLetra(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool ehDigito(char c) {
    return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

// Adiciona token na fita e na tabela de simbolos ao mesmo tempo.
// Evita duplicacao de codigo no loop principal.
void adicionarToken(
    std::vector<std::string>& fita,
    std::vector<RegistroToken>& simbolos,
    int linha,
    const std::string& identificador,
    const std::string& rotulo
) {
    fita.push_back(identificador);
    simbolos.push_back({linha, identificador, rotulo});
}

// Consome uma sequencia continua de caracteres que satisfaz o predicado.
// Exemplo:
// - se i aponta para inicio de "abc123" e predicado=ehLetra,
//   retorna "abc" e atualiza i para o '1'.
std::string consumirSequencia(const std::string& linha, std::size_t& i, bool (*predicado)(char)) {
    const std::size_t inicio = i;
    while (i < linha.size() && predicado(linha[i])) {
        ++i;
    }
    return linha.substr(inicio, i - inicio);
}
}  // namespace

AnalisadorLexico::AnalisadorLexico() = default;

std::string AnalisadorLexico::classificarPalavraComAFD(const std::string& lexema) const {
    // Comeca no estado inicial e caminha no AFD para cada caractere.
    std::string estado = ESTADO_INICIAL;

    for (char ch : lexema) {
        // Se o char nao pertence ao alfabeto do automato, tratamos como '@'.
        // Isso facilita mandar qualquer simbolo "fora do modelo" para erro.
        const char simbolo = ALFABETO.count(ch) ? ch : '@';

        // Busca o mapa de transicoes do estado atual.
        const auto itEstado = AFD.find(estado);
        if (itEstado == AFD.end()) {
            // Estado inexistente na tabela -> erro estrutural/entrada invalida.
            return "E99";
        }

        // Busca a transicao para o simbolo lido.
        const auto itTransicao = itEstado->second.find(simbolo);
        if (itTransicao == itEstado->second.end()) {
            // Transicao ausente -> token invalido para este automato.
            return "E99";
        }

        // Anda para o proximo estado.
        estado = itTransicao->second;
        if (estado == ESTADO_ERRO) {
            // Caiu no estado de erro: encerra classificacao.
            return "E99";
        }
    }

    // Depois de consumir todo lexema, precisa terminar em estado final.
    const auto itFinal = ESTADOS_FINAIS.find(estado);
    return itFinal != ESTADOS_FINAIS.end() ? itFinal->second : "E99";
}

std::string AnalisadorLexico::classificarLexema(const std::string& lexema) const {
    // Descobre se todos os chars sao digitos e/ou letras.
    // Comecamos com true e derrubamos para false quando achar violacao.
    bool soDigitos = !lexema.empty();
    bool soLetras = !lexema.empty();

    for (unsigned char c : lexema) {
        if (!std::isdigit(c)) {
            soDigitos = false;
        }
        if (!std::isalpha(c)) {
            soLetras = false;
        }
    }

    if (soDigitos) {
        // Constante inteira.
        return "E5";
    }

    if (soLetras) {
        // Palavra/identificador reconhecido pelo AFD.
        return classificarPalavraComAFD(lexema);
    }

    // Qualquer outro padrao (mistura invalida etc.)
    return "E99";
}

std::pair<std::vector<std::string>, std::vector<RegistroToken>> AnalisadorLexico::analisar(const std::string& fonte) const {
    // fita: sequencia de identificadores de token (E1 E2 ... $)
    std::vector<std::string> fita;
    // simbolos: tabela de simbolos (linha, token, lexema)
    std::vector<RegistroToken> simbolos;

    // istringstream permite ler string como se fosse arquivo.
    std::istringstream entrada(fonte);
    std::string linha;
    int numeroLinha = 0;

    // Leitura linha a linha para preencher coluna "linha" da tabela.
    while (std::getline(entrada, linha)) {
        ++numeroLinha;
        // i aponta para o caractere atual dentro da linha.
        std::size_t i = 0;

        while (i < linha.size()) {
            char ch = linha[i];

            // Ignora espacamentos (espaco, tab etc.).
            if (std::isspace(static_cast<unsigned char>(ch))) {
                ++i;
                continue;
            }

            // Verifica se e simbolo especial usando unordered_map.
            // find retorna iterador. Se != end(), existe a chave.
            const auto itEspecial = SIMBOLOS_ESPECIAIS.find(ch);
            if (itEspecial != SIMBOLOS_ESPECIAIS.end()) {
                // itEspecial->second e o token id associado ao simbolo.
                adicionarToken(fita, simbolos, numeroLinha, itEspecial->second, std::string(1, ch));
                ++i;
                continue;
            }

            // Bloco de letras: consome sequencia inteira [a-zA-Z]+.
            if (ehLetra(ch)) {
                const std::string lexema = consumirSequencia(linha, i, ehLetra);
                const std::string id = classificarLexema(lexema);
                adicionarToken(fita, simbolos, numeroLinha, id, lexema);
                continue;
            }

            // Bloco de digitos: consome sequencia inteira [0-9]+.
            if (ehDigito(ch)) {
                const std::string lexema = consumirSequencia(linha, i, ehDigito);
                const std::string id = classificarLexema(lexema);
                adicionarToken(fita, simbolos, numeroLinha, id, lexema);
                continue;
            }

            // Qualquer outro caractere isolado vira erro lexico.
            adicionarToken(fita, simbolos, numeroLinha, "E99", std::string(1, ch));
            ++i;
        }
    }

    // Marcador de fim da fita exigido no enunciado.
    fita.push_back("$");
    return {fita, simbolos};
}

std::string formatarTabelaSimbolos(const std::vector<RegistroToken>& simbolos) {
    // Monta texto TSV (tab-separated values) para ficar facil de ler/importar.
    std::ostringstream saida;
    saida << "linha\tidentificador\trotulo\n";

    for (const auto& s : simbolos) {
        saida << s.linha << "\t" << s.identificador << "\t" << s.rotulo << "\n";
    }

    return saida.str();
}
