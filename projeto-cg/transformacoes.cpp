#include "transformacoes.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<Ponto2D> transladar(const std::vector<Ponto2D>& pts, float dx, float dy) {
    std::vector<Ponto2D> res = pts;
    for (auto& p : res) {
        p.x += dx;
        p.y += dy;
    }
    return res;
}

std::vector<Ponto2D> rotacionar(const std::vector<Ponto2D>& pts, float anguloGraus) {
    std::vector<Ponto2D> res = pts;
    float rad = anguloGraus * (float)M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);

    for (auto& p : res) {
        float xNovo = p.x * cosA - p.y * sinA;
        float yNovo = p.x * sinA + p.y * cosA;
        p.x = xNovo;
        p.y = yNovo;
    }
    return res;
}

std::vector<Ponto2D> escalar(const std::vector<Ponto2D>& pts, float sx, float sy) {
    std::vector<Ponto2D> res = pts;
    for (auto& p : res) {
        p.x *= sx;
        p.y *= sy;
    }
    return res;
}
