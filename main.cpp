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

int main() {
    // fonte guardara todo o conteudo que sera analisado.
    std::string fonte;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tabelaSimbolos;
    std::unordered_map<int, int> tamanhoProducoes;
    

    {
        // Tenta abrir input.txt no diretorio atual.
        std::ifstream arquivoEntrada("input.txt");
        if (arquivoEntrada) {
            // Se abriu, le o arquivo inteiro para a string "fonte".
            std::ostringstream buffer;
            buffer << arquivoEntrada.rdbuf();
            fonte = buffer.str();
        } else {
            // Fallback para facilitar teste rapido sem arquivo.
            fonte = "se x = 10;\nfoi sai eaf;\n";
        }
    }

    // Executa o analisador lexico.
    AnalisadorLexico analisador;
    auto [fita, tabela] = analisador.analisar(fonte);

    // Imprime FITA no terminal.
    std::cout << "FITA: ";
    for (std::size_t i = 0; i < fita.size(); ++i) {
        std::cout << fita[i];
        if (i + 1 < fita.size()) {
            std::cout << ' ';
        }
    }
    std::cout << "\n\n";

    // Imprime Tabela de Simbolos no terminal.
    std::cout << "TABELA DE SIMBOLOS:\n";
    std::string tabelaTexto = formatarTabelaSimbolos(tabela, tabelaSimbolos);
    std::cout << tabelaTexto;

    // Garante que a pasta output exista.
    std::filesystem::create_directories("output");

    {
        // Salva a fita em arquivo texto.
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
        // Salva tabela em formato TSV (tabulado).
        std::ofstream tabelaArquivo("output/tabela_simbolos.tsv");
        tabelaArquivo << tabelaTexto;
    }

    // SLR Parsing Table as unordered_map
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

    return 0;
}

std::string analisadorSintatico(const std::vector<std::string>& fita, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& parsingTable, std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tabelaSimbolos) {
    //gera a fita de entrada para o analisador sintatico, e a tabela de parsing
    std::stack<std::string> pilha;
    pilha.push("0"); // estado inicial
    int linhas = tabelaSimbolos.size();
    for (size_t i = 0; i < linhas; i++){
        std::string simboloEntrada = fita[i];
        std::string estadoAtual = pilha.top();
        if (parsingTable.count(estadoAtual) > 0 && parsingTable.at(estadoAtual).count(simboloEntrada) > 0) {
            std::string acao = parsingTable.at(estadoAtual).at(simboloEntrada);
            if (acao[0] == 's') {
                // Shift
                pilha.push(simboloEntrada);
                pilha.push(acao.substr(1)); // nova estado
            } else if (acao[0] == 'r') {
                // Reduce
                int producao = std::stoi(acao.substr(1));
                // std::unordered_map<int, int> tamanhoProducoes deve conter o numero de simbolos a serem desempilhados para cada producao
                // precisamos ainda colocar o numero de simbolos a serem desempilhados para cada producao na tabela tamanhoProducoes
                // dai usar isso para desempilhar a quantidade correta de simbolos da pilha

            } else if (acao == "acc") {
                // Accept
                return "Análise sintática bem-sucedida.";
            }
        } else {
            return "Erro de análise sintática.";
        }
    }

    return "Análise sintática não implementada.";
}

