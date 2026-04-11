#include "lexer.hpp"

// filesystem: criar pasta output de forma portavel
#include <filesystem>
// fstream: leitura/escrita de arquivos
#include <fstream>
// iostream: saida no terminal
#include <iostream>
// sstream: ler arquivo inteiro para string
#include <sstream>

int main() {
    // fonte guardara todo o conteudo que sera analisado.
    std::string fonte;

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
    std::string tabelaTexto = formatarTabelaSimbolos(tabela);
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

    return 0;
}
