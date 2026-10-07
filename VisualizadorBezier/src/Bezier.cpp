#include "Bezier.h"

Ponto calcularBezier(
    const Ponto& p0,
    const Ponto& p1,
    const Ponto& p2,
    const Ponto& p3,
    double t
) {
    const double u = 1.0 - t;

    // Polinômios de Bernstein de grau 3
    const double a = u * u * u;
    const double b = 3.0 * u * u * t;
    const double c = 3.0 * u * t * t;
    const double d = t * t * t;

    return {
        a * p0.x + b * p1.x + c * p2.x + d * p3.x,
        a * p0.y + b * p1.y + c * p2.y + d * p3.y
    };
}
