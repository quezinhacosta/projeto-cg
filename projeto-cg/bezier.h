
#include <vector>
#include "Ponto.h"

// Calcula e desenha curvas de Bezier cubicas concatenadas
void desenharCurvasBezier(const std::vector<Ponto2D>& pontos, int resolucaoSegmento = 50);

// Desenha as retas e os vertices do poligono de controle
void desenharPoligonoDeControle(const std::vector<Ponto2D>& pontos);