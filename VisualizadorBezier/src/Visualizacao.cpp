#include "Visualizacao.h"
#include "Transformacoes.h"
#include "Menu.h"

#include "Bezier.h"
#include "Ponto.h"

#include <algorithm>
#include <vector>

#include <GL/glut.h>

// ==========================================
// Variáveis externas
// ==========================================

extern std::vector<Ponto> pontosControle;

extern bool exibirPoligono;
extern bool exibirTransformada;

extern double esquerda;
extern double direita;
extern double baixo;
extern double cima;

// ==========================================
// Eixos X e Y
// ==========================================

void desenharEixos()
{
    glLineWidth(2.0f);

    // Eixo X - verde
    glColor3f(0.0f, 0.7f, 0.0f);

    glBegin(GL_LINES);

    glVertex2d(esquerda, 0.0);
    glVertex2d(direita, 0.0);

    glEnd();

    // Eixo Y - azul
    glColor3f(0.0f, 0.2f, 1.0f);

    glBegin(GL_LINES);

    glVertex2d(0.0, baixo);
    glVertex2d(0.0, cima);

    glEnd();
}

// ==========================================
// Polígono de controle
// ==========================================

void desenharPoligonoDeControle()
{
    if (pontosControle.empty()) {
        return;
    }

    // Arestas do polígono
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);

    glBegin(GL_LINE_STRIP);

    for (const auto& pt : pontosControle) {
        glVertex2d(pt.x, pt.y);
    }

    glEnd();

    // Pontos de controle
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(6.0f);

    glBegin(GL_POINTS);

    for (const auto& pt : pontosControle) {
        glVertex2d(pt.x, pt.y);
    }

    glEnd();
}

// ==========================================
// Desenho principal
// ==========================================

void desenhar()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ==========================================
    // Eixos
    // ==========================================

    desenharEixos();

    if (!pontosControle.empty()) {

        // ==========================================
        // CURVA ORIGINAL
        // ==========================================

        if (exibirPoligono) {
            desenharPoligonoDeControle();
        }

        glColor3f(0.9f, 0.1f, 0.1f);
        glLineWidth(2.5f);

        desenharCurvaBezier();


        // ==========================================
        // CURVA TRANSFORMADA
        // ==========================================

        if (exibirTransformada) {

            glPushMatrix();

            aplicarTransformacoesAcumuladas();

            if (exibirPoligono) {
                desenharPoligonoDeControle();
            }

            glColor3f(0.1f, 0.3f, 0.9f);
            glLineWidth(2.5f);

            desenharCurvaBezier();

            glPopMatrix();
        }
    }

    glutSwapBuffers();
}

// ==========================================
// Redimensionamento da janela
// ==========================================

void redimensionar(int largura, int altura)
{
    largura = std::max(largura, 1);
    altura = std::max(altura, 1);

    glViewport(0, 0, largura, altura);

    // ==========================================
    // Calcula os limites dos pontos
    // ==========================================

    double minX = 0.0;
    double maxX = 0.0;
    double minY = 0.0;
    double maxY = 0.0;

    for (const Ponto& ponto : pontosControle) {

        minX = std::min(minX, ponto.x);
        maxX = std::max(maxX, ponto.x);

        minY = std::min(minY, ponto.y);
        maxY = std::max(maxY, ponto.y);
    }

    const double larguraBase =
        std::max(maxX - minX, 20.0);

    const double alturaBase =
        std::max(maxY - minY, 20.0);

    const double centroX =
        (minX + maxX) / 2.0;

    const double centroY =
        (minY + maxY) / 2.0;

    double larguraVisivel =
        larguraBase * 1.2;

    double alturaVisivel =
        alturaBase * 1.2;

    const double proporcaoJanela =
        static_cast<double>(largura) / altura;

    if (larguraVisivel / alturaVisivel < proporcaoJanela) {

        larguraVisivel =
            alturaVisivel * proporcaoJanela;
    }
    else {

        alturaVisivel =
            larguraVisivel / proporcaoJanela;
    }

    esquerda =
        centroX - larguraVisivel / 2.0;

    direita =
        centroX + larguraVisivel / 2.0;

    baixo =
        centroY - alturaVisivel / 2.0;

    cima =
        centroY + alturaVisivel / 2.0;

    // ==========================================
    // Projeção
    // ==========================================

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(
        esquerda,
        direita,
        baixo,
        cima
    );

    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}
