#include "ArquivoOBJ.h"
#include "Ponto.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

extern std::vector<Ponto> pontosControle;

bool carregarObj(const std::string& caminho) {
    std::ifstream arquivo(caminho);

    if (!arquivo) {
        std::cerr << "Nao foi possivel abrir: " << caminho << '\n';
        return false;
    }

    std::vector<Ponto> pontos;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        std::istringstream leitor(linha);
        std::string tipo;

        leitor >> tipo;

        if (tipo == "v") {
            Ponto ponto;

            if (leitor >> ponto.x >> ponto.y) {
                pontos.push_back(ponto);
            }
        }

    }

    if (pontos.empty()) {
        std::cerr
            << "O arquivo nao possui pontos de controle.\n";

        return false;
    }

    std::cout << "Quantidade de pontos carregados: "
            << pontos.size() << "\n";


    pontosControle = pontos;

    std::cout << "Arquivo carregado: " << caminho << '\n';

    return true;
}


bool salvarObj(const std::string& caminho) {
    if (pontosControle.empty()) {
        std::cerr << "Nenhum ponto de controle para salvar.\n";
        return false;
    }

    std::ofstream arquivo(caminho);

    if (!arquivo) {
        std::cerr
            << "Erro ao criar o arquivo: "
            << caminho << '\n';

        return false;
    }

    arquivo << "# Arquivo gerado pelo Visualizador de Curvas de Bezier\n";
    arquivo << "# Pontos de controle da curva\n\n";

    for (const auto& pt : pontosControle) {
        arquivo << "v "
                << pt.x << " "
                << pt.y << "\n";
    }

    std::cout
        << "Figura salva com sucesso em: "
        << caminho << '\n';

    return true;
}