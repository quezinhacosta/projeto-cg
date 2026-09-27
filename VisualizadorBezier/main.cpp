#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <GL/glui.h>

struct Ponto {
    double x;
    double y;
};

std::vector<Ponto> pontosControle;

double esquerda = -100.0;
double direita = 100.0;
double baixo = -100.0;
double cima = 100.0;

bool carregarObj(const std::string& caminho) {
    std::ifstream arquivo(caminho);

    if (!arquivo) {
        std::cerr << "Nao foi possivel abrir: " << caminho << '\n';
        return false;
    }

    std::vector<Ponto> pontos;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        std::istringstream leitor(linha);
        std::string tipo;

        leitor >> tipo;

        if (tipo == "v") {
            Ponto ponto;

            if (leitor >> ponto.x >> ponto.y) {
                pontos.push_back(ponto);
            }
        }
        // Linhas vazias e comentarios iniciados por # sao ignorados.
    }

    if (pontos.size() != 4) {
        std::cerr << "Este primeiro teste precisa de exatamente 4 pontos; "
            << "o arquivo contem " << pontos.size() << ".\n";
        return false;
    }

    pontosControle = pontos;
    std::cout << "Arquivo carregado: " << caminho << '\n';
    return true;
}

Ponto calcularBezier(double t) {
    const double u = 1.0 - t;

    const double a = u * u * u;
    const double b = 3.0 * u * u * t;
    const double c = 3.0 * u * t * t;
    const double d = t * t * t;

    return {
        a * pontosControle[0].x + b * pontosControle[1].x
          + c * pontosControle[2].x + d * pontosControle[3].x,

        a * pontosControle[0].y + b * pontosControle[1].y
          + c * pontosControle[2].y + d * pontosControle[3].y
    };
}

void desenhar() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(2.0f);

    // Eixo X: verde.
    glColor3f(0.0f, 0.7f, 0.0f);
    glBegin(GL_LINES);
    glVertex2d(esquerda, 0.0);
    glVertex2d(direita, 0.0);
    glEnd();

    // Eixo Y: azul.
    glColor3f(0.0f, 0.2f, 1.0f);
    glBegin(GL_LINES);
    glVertex2d(0.0, baixo);
    glVertex2d(0.0, cima);
    glEnd();

    // Curva: vermelha.
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);

    const int amostras = 100;
    for (int i = 0; i <= amostras; ++i) {
        const double t = static_cast<double>(i) / amostras;
        const Ponto ponto = calcularBezier(t);
        glVertex2d(ponto.x, ponto.y);
    }

    glEnd();
    glutSwapBuffers();
}

void redimensionar(int largura, int altura) {
    largura = std::max(largura, 1);
    altura = std::max(altura, 1);

    glViewport(0, 0, largura, altura);

    // Inclui a origem para que ambos os eixos continuem visiveis.
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

    // Evita uma area de visualizacao pequena demais.
    const double larguraBase = std::max(maxX - minX, 20.0);
    const double alturaBase = std::max(maxY - minY, 20.0);

    const double centroX = (minX + maxX) / 2.0;
    const double centroY = (minY + maxY) / 2.0;

    // Acrescenta margem e expande uma das dimensoes conforme a janela.
    double larguraVisivel = larguraBase * 1.2;
    double alturaVisivel = alturaBase * 1.2;

    const double proporcaoJanela =
        static_cast<double>(largura) / altura;

    if (larguraVisivel / alturaVisivel < proporcaoJanela) {
        larguraVisivel = alturaVisivel * proporcaoJanela;
    }
    else {
        alturaVisivel = larguraVisivel / proporcaoJanela;
    }

    esquerda = centroX - larguraVisivel / 2.0;
    direita = centroX + larguraVisivel / 2.0;
    baixo = centroY - alturaVisivel / 2.0;
    cima = centroY + alturaVisivel / 2.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(esquerda, direita, baixo, cima);

    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int, int) {
    if (tecla == 'q' || tecla == 'Q' || tecla == 27) {
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    // Guardamos o caminho antes de glutInit, que pode processar argumentos.
    const std::string caminho =
        (argc > 1) ? argv[1] : "desenhos/teste.obj";

    if (!carregarObj(caminho)) {
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Visualizador de Curvas de Bezier");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(desenhar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);

    glutMainLoop();
    return 0;
}