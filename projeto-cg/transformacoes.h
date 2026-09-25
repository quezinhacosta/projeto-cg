#pragma once
#include <vector>
#include "Ponto.h"

// Aplica translacao (dx, dy)
std::vector<Ponto2D> transladar(const std::vector<Ponto2D>& pts, float dx, float dy);

// Aplica rotacao em graus em relacao a origem
std::vector<Ponto2D> rotacionar(const std::vector<Ponto2D>& pts, float anguloGraus);

// Aplica escala (sx, sy)
std::vector<Ponto2D> escalar(const std::vector<Ponto2D>& pts, float sx, float sy);