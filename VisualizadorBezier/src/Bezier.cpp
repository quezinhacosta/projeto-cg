
#include "Bezier.h"
#include "Ponto.h"

#include <vector>

#include <GL/glut.h>

extern std::vector<Ponto> pontosControle;

// ==========================================
// Bézier cúbica
// ==========================================

Ponto bezierCubica(
    const Ponto& p0,
    const Ponto& p1,
    const Ponto& p2,
    const Ponto& p3,
    double t
) {
    const double u = 1.0 - t;

    const double b0 = u * u * u;
    const double b1 = 3.0 * u * u * t;
    const double b2 = 3.0 * u * t * t;
    const double b3 = t * t * t;

    Ponto resultado;

    resultado.x =
        b0 * p0.x +
        b1 * p1.x +
        b2 * p2.x +
        b3 * p3.x;

    resultado.y =
        b0 * p0.y +
        b1 * p1.y +
        b2 * p2.y +
        b3 * p3.y;

    return resultado;
}

// ==========================================
// Desenhar curva usando vários segmentos
// de Bézier cúbica
// ==========================================

void desenharCurvaBezier()
{
    if (pontosControle.size() < 4) {
        return;
    }

    glBegin(GL_LINE_STRIP);

    for (std::size_t i = 0;
         i + 3 < pontosControle.size();
         i += 3) {

        const Ponto& p0 = pontosControle[i];
        const Ponto& p1 = pontosControle[i + 1];
        const Ponto& p2 = pontosControle[i + 2];
        const Ponto& p3 = pontosControle[i + 3];

        const int amostras = 50;

        for (int k = 0; k <= amostras; ++k) {

            const double t =
                static_cast<double>(k) / amostras;

            const Ponto ponto =
                bezierCubica(
                    p0,
                    p1,
                    p2,
                    p3,
                    t
                );

            glVertex2d(ponto.x, ponto.y);
        }
    }

    glEnd();
}
