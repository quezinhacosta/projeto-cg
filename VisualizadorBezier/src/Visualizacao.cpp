#include "Visualizacao.h"
#include "Transformacoes.h"
#include "Menu.h"

#include "Bezier.h"
#include "Ponto.h"

#include <algorithm>
#include <GL/glut.h>

<<<<<<< HEAD
// ==========================================
// Variáveis externas
// ==========================================

extern FiguraBezier figuraBezier;
=======
extern std::vector<Ponto> pontosControle;
>>>>>>> c92b0443e6a4adeef92d955a5760c60f107e95c0

extern bool exibirPoligono;
extern bool exibirTransformada;

extern double esquerda;
extern double direita;
extern double baixo;
extern double cima;

void desenharEixos()
{
    glLineWidth(2.0f);

    glColor3f(0.0f, 0.7f, 0.0f);

    glBegin(GL_LINES);

    glVertex2d(esquerda, 0.0);
    glVertex2d(direita, 0.0);

    glEnd();

    glColor3f(0.0f, 0.2f, 1.0f);

    glBegin(GL_LINES);

    glVertex2d(0.0, baixo);
    glVertex2d(0.0, cima);

    glEnd();
}

<<<<<<< HEAD
// ==========================================
// Polígono de controle
// ==========================================

void desenharPoligonoDeControle() {
    if (figuraBezier.empty()) {
=======
void desenharPoligonoDeControle()
{
    if (pontosControle.empty()) {
>>>>>>> c92b0443e6a4adeef92d955a5760c60f107e95c0
        return;
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);

    for (const auto& contorno : figuraBezier) {
        glBegin(GL_LINE_STRIP);
        for (const auto& pt : contorno) {
            glVertex2d(pt.x, pt.y);
        }
        glEnd();
    }

<<<<<<< HEAD
    // Vértices de controle
=======
    glEnd();

>>>>>>> c92b0443e6a4adeef92d955a5760c60f107e95c0
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(6.0f);

    glBegin(GL_POINTS);

    for (const auto& contorno : figuraBezier) {
        for (const auto& pt : contorno) {
            glVertex2d(pt.x, pt.y);
        }
    }

    glEnd();
}

void desenhar()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenharEixos();

    if (!figuraBezier.empty()) {

        if (exibirPoligono) {
            desenharPoligonoDeControle();
        }

        glColor3f(0.9f, 0.1f, 0.1f);
        glLineWidth(2.5f);

        desenharCurvaBezier();


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

void redimensionar(int largura, int altura)
{
    largura = std::max(largura, 1);
    altura = std::max(altura, 1);

    glViewport(0, 0, largura, altura);

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
