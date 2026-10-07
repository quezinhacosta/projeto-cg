#include "ArquivoOBJ.h"
#include "Ponto.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Acesso aos pontos de controle definidos no programa principal
extern std::vector<Ponto> pontosControle;

// ==========================================
// Carregar arquivo OBJ
// ==========================================

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

        // Linhas que indicam vertices com coordenadas (x, y)
        if (tipo == "v") {
            Ponto ponto;

            if (leitor >> ponto.x >> ponto.y) {
                pontos.push_back(ponto);
            }
        }

        // Comentarios (#) e linhas vazias sao ignorados
    }

    if (pontos.size() != 4) {
        std::cerr
            << "Este primeiro teste precisa de exatamente 4 pontos; "
            << "o arquivo contem " << pontos.size() << ".\n";

        return false;
    }

    // So substitui os pontos depois que o arquivo foi validado
    pontosControle = pontos;

    std::cout << "Arquivo carregado: " << caminho << '\n';

    return true;
}

// ==========================================
// Salvar arquivo OBJ
// ==========================================

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