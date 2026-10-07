#include "Bezier.h"

#include <vector>

// Acesso aos pontos de controle definidos no programa principal
extern std::vector<Ponto> pontosControle;

Ponto calcularBezier(double t) {
    const double u = 1.0 - t;

    // Polinômios de Bernstein de grau 3
    const double a = u * u * u;
    const double b = 3.0 * u * u * t;
    const double c = 3.0 * u * t * t;
    const double d = t * t * t;

    return {
        a * pontosControle[0].x
            + b * pontosControle[1].x
            + c * pontosControle[2].x
            + d * pontosControle[3].x,

        a * pontosControle[0].y
            + b * pontosControle[1].y
            + c * pontosControle[2].y
            + d * pontosControle[3].y
    };
}

