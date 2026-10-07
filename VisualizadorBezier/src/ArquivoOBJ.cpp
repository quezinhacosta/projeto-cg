#include "ArquivoOBJ.h"
#include "Ponto.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

// Acesso à estrutura de contornos definida no programa principal
extern FiguraBezier figuraBezier;

// ==========================================
// Carregar arquivo OBJ
// ==========================================

bool carregarObj(const std::string& caminho) {
    std::ifstream arquivo(caminho);

    if (!arquivo) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << caminho << '\n';
        return false;
    }

    FiguraBezier figura;
    ContornoBezier contornoAtual;
    std::string linha;
    std::size_t numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        ++numeroLinha;

        // Limpeza de espaços em branco iniciais
        std::istringstream leitor(linha);
        std::string tipo;
        
        if (!(leitor >> tipo) || tipo[0] == '#') {
            continue; // Linha vazia ou comentário
        }

        // Separação de objetos/grupos independentes (ex: pétalas, caule, folha)
        if (tipo == "o" || tipo == "g") {
            if (!contornoAtual.empty()) {
                figura.push_back(std::move(contornoAtual));
                contornoAtual.clear();
            }
            continue;
        }

        // Vértices 2D: v x y
        if (tipo == "v") {
            Ponto ponto;

            if (!(leitor >> ponto.x >> ponto.y)
                || !std::isfinite(ponto.x)
                || !std::isfinite(ponto.y)) {
                std::cerr << "Erro: Vertice invalido na linha "
                          << numeroLinha << " de " << caminho << ".\n";
                return false;
            }

            contornoAtual.push_back(ponto);
        }
    }

    // Adiciona o último contorno lido se não estiver vazio
    if (!contornoAtual.empty()) {
        figura.push_back(std::move(contornoAtual));
    }

    // Contabilização e validação das curvas
    std::size_t totalPontos = 0;
    std::size_t totalCurvas = 0;
    std::size_t totalPolilinhas = 0;

    for (const auto& contorno : figura) {
        totalPontos += contorno.size();
        if (contorno.size() >= 4 && (contorno.size() - 1) % 3 == 0) {
            totalCurvas += (contorno.size() - 1) / 3;
        } else {
            ++totalPolilinhas;
        }
    }

    if (totalPontos == 0) {
        std::cerr << "Erro: O arquivo nao contem vertices 'v x y': "
                  << caminho << ".\n";
        return false;
    }

   if (totalPontos < 300) {
        std::cout << "Aviso: O arquivo contem " << totalPontos
                  << " pontos de controle (a entrega final exige no minimo 300).\n";
    }

    // Substitui a figura apenas após validação completa com sucesso
    figuraBezier = std::move(figura);

    std::cout << "Arquivo carregado com sucesso: " << caminho
              << " (" << totalPontos << " pontos, "
              << totalCurvas << " trechos cubicos, "
              << totalPolilinhas << " polilinha(s) em "
              << figuraBezier.size() << " contorno(s))\n";

    return true;
}

// ==========================================
// Salvar arquivo OBJ
// ==========================================

bool salvarObj(const std::string& caminho) {
    if (figuraBezier.empty()) {
        std::cerr << "Erro: Nenhum ponto de controle para salvar.\n";
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