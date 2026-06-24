#include "lexer.hpp"

// filesystem: criar pasta output de forma portavel
#include <filesystem>
// fstream: leitura/escrita de arquivos
#include <fstream>
// iostream: saida no terminal
#include <iostream>
// sstream: ler arquivo inteiro para string
#include <sstream>

#include <unordered_map>

#include <string>

#include <stack>

#include<iostream>

std::string analisadorSintatico(const std::vector<std::string>& fita, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& parsingTable, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& tabelaSimbolos, std::unordered_map<int, int>& tamanhoProducoes);

int main() {
    std::string fonte;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tabelaSimbolos;

    //Para operações de redução, precisamos saber o tamanho do lado direito da produção.
    std::unordered_map<int, int> tamanhoProducoes = {
        {1, 3},
        {2, 1},
        {3, 3},
        {4, 1},
        {5, 3},
        {6, 1}
    };

    // Tabela de parsing para o analisador sintático.
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> parsingTable = {
        {"0", {{"+", " "}, {"*", " "}, {"(", "s4"}, {")", " "}, {"id", "s5"}, {"$", " "}, {"E'", " "}, {"E", "1"}, {"T", "2"}, {"F", "3"}}},
        {"1", {{"+", "s6"}, {"*", " "}, {"(", " "}, {")", " "}, {"id", " "}, {"$", "acc"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"2", {{"+", "r2"}, {"*", "s7"}, {"(", " "}, {")", "r2"}, {"id", " "}, {"$", "r2"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"3", {{"+", "r4"}, {"*", "r4"}, {"(", " "}, {")", "r4"}, {"id", " "}, {"$", "r4"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"4", {{"+", " "}, {"*", " "}, {"(", "s4"}, {")", " "}, {"id", "s5"}, {"$", " "}, {"E'", " "}, {"E", "8"}, {"T", "2"}, {"F", "3"}}},
        {"5", {{"+", "r6"}, {"*", "r6"}, {"(", " "}, {")", "r6"}, {"id", " "}, {"$", "r6"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"6", {{"+", " "}, {"*", " "}, {"(", "s4"}, {")", " "}, {"id", "s5"}, {"$", " "}, {"E'", " "}, {"E", " "}, {"T", "9"}, {"F", "3"}}},
        {"7", {{"+", " "}, {"*", " "}, {"(", "s4"}, {")", " "}, {"id", "s5"}, {"$", " "}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", "10"}}},
        {"8", {{"+", "s6"}, {"*", " "}, {"(", " "}, {")", "s11"}, {"id", " "}, {"$", " "}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"9", {{"+", "r1"}, {"*", "s7"}, {"(", " "}, {")", "r1"}, {"id", " "}, {"$", "r1"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"10", {{"+", "r3"}, {"*", "r3"}, {"(", " "}, {")", "r3"}, {"id", " "}, {"$", "r3"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}},
        {"11", {{"+", "r5"}, {"*", "r5"}, {"(", " "}, {")", "r5"}, {"id", " "}, {"$", "r5"}, {"E'", " "}, {"E", " "}, {"T", " "}, {"F", " "}}}
    };

    // Tenta abrir o arquivo de entrada "input.txt". Se não conseguir, usa uma string de teste.
    std::ifstream arquivoEntrada("input.txt");
    if (arquivoEntrada) {
        std::ostringstream buffer;
        buffer << arquivoEntrada.rdbuf();
        fonte = buffer.str();
    } else {
        fonte = "x+x*y\n";
    }

    AnalisadorLexico analisador;
    auto [fita, tabela] = analisador.analisar(fonte);
    std::string resultadoSintatico = analisadorSintatico(fita, parsingTable, tabelaSimbolos, tamanhoProducoes);


    std::cout << "FITA: ";
    for (std::size_t i = 0; i < fita.size(); ++i) {
        std::cout << fita[i];
        if (i + 1 < fita.size()) {
            std::cout << ' ';
        }
    }
    std::cout << "\n\n";

    std::cout << "TABELA DE SIMBOLOS:\n";
    std::string tabelaTexto = formatarTabelaSimbolos(tabela, tabelaSimbolos);
    std::cout << tabelaTexto;

    std::cout << "\nRESULTADO SINTATICO:\n" << resultadoSintatico << "\n";

    std::filesystem::create_directories("output");

    {
        std::ofstream fitaArquivo("output/fita.txt");
        for (std::size_t i = 0; i < fita.size(); ++i) {
            fitaArquivo << fita[i];
            if (i + 1 < fita.size()) {
                fitaArquivo << ' ';
            }
        }
        fitaArquivo << '\n';
    }

    {
        std::ofstream tabelaArquivo("output/tabela_simbolos.tsv");
        tabelaArquivo << tabelaTexto;
    }

    return 0;
}

std::string analisadorSintatico(const std::vector<std::string>& fita, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& parsingTable, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& tabelaSimbolos, std::unordered_map<int, int>& tamanhoProducoes) {
    (void)tabelaSimbolos;

    std::stack<std::string> pilha;
    pilha.push("0");

    const std::unordered_map<int, std::string> producaoParaNaoTerminal = {
        {1, "E"},
        {2, "E"},
        {3, "T"},
        {4, "T"},
        {5, "F"},
        {6, "F"}
    };

    for (std::size_t i = 0; i < fita.size(); ++i) {
        const std::string& simboloEntrada = fita[i];

        while (true) {
            std::string estadoAtual = pilha.top();
            if (parsingTable.count(estadoAtual) == 0 || parsingTable.at(estadoAtual).count(simboloEntrada) == 0) {
                return "Erro de analise sintatica.";
            }

            std::string acao = parsingTable.at(estadoAtual).at(simboloEntrada);
            if (acao == " ") {
                return "Erro de analise sintatica.";
            }

            if (acao == "acc") {
                return "Analise sintatica bem-sucedida.";
            }

            if (acao[0] == 's') {
                pilha.push(simboloEntrada);
                pilha.push(acao.substr(1));
                break;
            }

            if (acao[0] == 'r') {
                int producao = std::stoi(acao.substr(1));
                int tamanho = tamanhoProducoes[producao];
                for (int j = 0; j < tamanho; ++j) {
                    if (pilha.size() < 2) {
                        return "Erro de analise sintatica.";
                    }
                    pilha.pop();
                    pilha.pop();
                }

                std::string naoTerminal = producaoParaNaoTerminal.at(producao);
                std::string estadoTopo = pilha.top();
                if (parsingTable.count(estadoTopo) == 0 || parsingTable.at(estadoTopo).count(naoTerminal) == 0) {
                    return "Erro de analise sintatica.";
                }

                std::string proximoEstado = parsingTable.at(estadoTopo).at(naoTerminal);
                if (proximoEstado == " ") {
                    return "Erro de analise sintatica.";
                }

                pilha.push(naoTerminal);
                pilha.push(proximoEstado);
                continue;
            }

            return "Erro de analise sintatica.";
        }
    }

    return "Analise sintatica nao implementada.";
}
