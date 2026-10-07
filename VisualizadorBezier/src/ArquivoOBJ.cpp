#include "ArquivoOBJ.h"
#include "Ponto.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

extern FiguraBezier figuraBezier;

// ==========================================
// Carregar arquivo OBJ
// ==========================================

bool carregarObj(const std::string& caminho) {
    std::ifstream arquivo(caminho);

    if (!arquivo) {
        std::cerr << "Nao foi possivel abrir: " << caminho << '\n';
        return false;
    }

    FiguraBezier figura;
    ContornoBezier contornoAtual;
    std::string linha;
    std::size_t numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        ++numeroLinha;
        std::istringstream leitor(linha);
        std::string tipo;
        if (!(leitor >> tipo) || tipo[0] == '#') {
            continue;
        }

        if (tipo == "o" || tipo == "g") {
            if (!contornoAtual.empty()) {
                figura.push_back(std::move(contornoAtual));
                contornoAtual.clear();
            }
            continue;
        }

        if (tipo == "v") {
            Ponto ponto;

            if (!(leitor >> ponto.x >> ponto.y)
                || !std::isfinite(ponto.x)
                || !std::isfinite(ponto.y)) {
                std::cerr << "Erro: vertice invalido na linha "
                          << numeroLinha << " de " << caminho << ".\n";
                return false;
            }

            contornoAtual.push_back(ponto);
        }
    }

    if (!contornoAtual.empty()) {
        figura.push_back(std::move(contornoAtual));
    }

    std::size_t totalPontos = 0;
    std::size_t totalCurvas = 0;
    for (std::size_t i = 0; i < figura.size(); ++i) {
        const std::size_t quantidade = figura[i].size();
        totalPontos += quantidade;

        if (quantidade < 4 || (quantidade - 1) % 3 != 0) {
            std::cerr << "Erro: o contorno " << (i + 1) << " de " << caminho
                      << " possui " << quantidade
                      << " pontos; cada contorno deve ter ao menos 4 pontos "
                         "e obedecer a regra 3k + 1.\n";
            return false;
        }

        totalCurvas += (quantidade - 1) / 3;
    }

    if (totalPontos < 4) {
        std::cerr << "Erro: o arquivo precisa conter ao menos 4 pontos de controle.\n";
        return false;
    }

    if (totalPontos < 300) {
        std::cout << "Aviso: o arquivo contem " << totalPontos
                  << " pontos de controle (a entrega final exige no minimo 300).\n";
    }

    figuraBezier = std::move(figura);

    std::cout << "Arquivo carregado com sucesso: " << caminho
              << " (" << totalPontos << " pontos, "
              << totalCurvas << " trechos cubicos em "
              << figuraBezier.size() << " contorno(s))\n";

    return true;
}

// ==========================================
// Salvar arquivo OBJ
// ==========================================

bool salvarObj(const std::string& caminho) {
    if (figuraBezier.empty()) {
        std::cerr << "Nenhum ponto de controle para salvar.\n";
        return false;
    }

    std::ofstream arquivo(caminho);

    if (!arquivo) {
        std::cerr << "Erro ao criar o arquivo: " << caminho << '\n';
        return false;
    }

    arquivo << "# Arquivo gerado pelo Visualizador de Curvas de Bezier\n";
    arquivo << "# Cada objeto representa um contorno Bezier independente\n\n";

    for (std::size_t i = 0; i < figuraBezier.size(); ++i) {
        arquivo << "o contorno_" << (i + 1) << "\n";
        for (const auto& pt : figuraBezier[i]) {
            arquivo << "v " << pt.x << " " << pt.y << "\n";
        }
        arquivo << "\n";
    }

    std::cout << "Figura salva com sucesso em: " << caminho << '\n';

    return true;
}