#include "Bezier.h"
#include <cmath>

#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif

// Avalia um ponto P(u) em uma curva de Bezier cubica com u no intervalo [0, 1]
static Ponto2D calcularPontoCubico(float u, Ponto2D p0, Ponto2D p1, Ponto2D p2, Ponto2D p3) {
    float um_u = 1.0f - u;

    // Polinomios de Bernstein para n = 3:
    // B0(u) = (1-u)^3
    // B1(u) = 3u(1-u)^2
    // B2(u) = 3u^2(1-u)
    // B3(u) = u^3
    float b0 = um_u * um_u * um_u;
    float b1 = 3.0f * u * um_u * um_u;
    float b2 = 3.0f * u * u * um_u;
    float b3 = u * u * u;

    Ponto2D p;
    p.x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
    p.y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;
    return p;
}

void desenharCurvasBezier(const std::vector<Ponto2D>& pontos, int resolucaoSegmento) {
    if (pontos.size() < 4) return;

    glLineWidth(2.5f);
    glBegin(GL_LINES);

    // Itera pelos pontos de 3 em 3 para curvas concatenadas (p0, p1, p2, p3), (p3, p4, p5, p6)...
    for (size_t i = 0; i + 3 < pontos.size(); i += 3) {
        Ponto2D p0 = pontos[i];
        Ponto2D p1 = pontos[i + 1];
        Ponto2D p2 = pontos[i + 2];
        Ponto2D p3 = pontos[i + 3];

        Ponto2D anterior = p0;
        for (int step = 1; step <= resolucaoSegmento; ++step) {
            float u = (float)step / (float)resolucaoSegmento;
            Ponto2D atual = calcularPontoCubico(u, p0, p1, p2, p3);

            glVertex2f(anterior.x, anterior.y);
            glVertex2f(atual.x, atual.y);

            anterior = atual;
        }
    }
    glEnd();
}

void desenharPoligonoDeControle(const std::vector<Ponto2D>& pontos) {
    if (pontos.empty()) return;

    // Segmentos conectando os vertices
    glColor3f(0.55f, 0.55f, 0.55f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& pt : pontos) {
        glVertex2f(pt.x, pt.y);
    }
    glEnd();

    // Vertices destacados com pontos
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto& pt : pontos) {
        glVertex2f(pt.x, pt.y);
    }
    glEnd();
}