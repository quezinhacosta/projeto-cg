#include "ArquivoOBJ.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool carregarOBJ(const std::string& nomeArquivo, std::vector<Ponto2D>& pontos) {
    std::ifstream file(nomeArquivo);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << nomeArquivo << std::endl;
        return false;
    }

    pontos.clear();
    std::string linha;

    while (std::getline(file, linha)) {
        if (linha.empty()) continue;

        // Comentarios iniciados por '#' sao ignorados conforme secao 2
        if (linha[0] == '#') continue;

        std::istringstream ss(linha);
        std::string prefixo;
        ss >> prefixo;

        // Linhas iniciadas com 'v' indicam vertice com coordenadas x e y
        if (prefixo == "v") {
            float x, y;
            if (ss >> x >> y) {
                pontos.emplace_back(x, y);
            }
        }
    }

    file.close();
    std::cout << "Carregados " << pontos.size() << " pontos do arquivo: " << nomeArquivo << std::endl;
    return true;
}

bool salvarOBJ(const std::string& nomeArquivo, const std::vector<Ponto2D>& pontos) {
    std::ofstream file(nomeArquivo);
    if (!file.is_open()) {
        std::cerr << "Erro ao gravar no arquivo: " << nomeArquivo << std::endl;
        return false;
    }

    file << "# Arquivo gerado pelo Visualizador de Curvas de Bezier\n";
    file << "# Quantidade de vertices: " << pontos.size() << "\n\n";

    for (const auto& pt : pontos) {
        file << "v " << pt.x << " " << pt.y << "\n";
    }

    file.close();
    std::cout << "Figura salva com sucesso em: " << nomeArquivo << std::endl;
    return true;
}