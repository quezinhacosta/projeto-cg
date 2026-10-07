#include <iostream>
#include <string>

#include <GL/glut.h>

#include "Ponto.h"
#include "ArquivoOBJ.h"
#include "Bezier.h"
#include "Menu.h"
#include "Visualizacao.h"
#include "Transformacoes.h"

// ==========================================
// Variáveis globais do programa
// ==========================================

// Figura composta por contornos Bezier independentes
FiguraBezier figuraBezier;

// Controle de visualização
double esquerda = -100.0;
double direita  = 100.0;
double baixo    = -100.0;
double cima     = 100.0;

// ==========================================
// Função principal
// ==========================================

int main(int argc, char** argv)
{
    // ------------------------------------------
    // Caminho inicial do arquivo OBJ
    // ------------------------------------------

    const std::string caminho =
        (argc > 1)
            ? argv[1]
            : "desenhos/rosa.obj";

    // ------------------------------------------
    // Carrega o arquivo OBJ
    // ------------------------------------------

    if (!carregarObj(caminho)) {
        return 1;
    }

    // ------------------------------------------
    // Inicialização do GLUT
    // ------------------------------------------

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGB
    );

    glutInitWindowSize(
        800,
        600
    );

    // ------------------------------------------
    // Criação da janela principal
    // ------------------------------------------

    janelaVisualizacao =
        glutCreateWindow(
            "Visualizador de Curvas de Bezier"
        );

    // ------------------------------------------
    // Configuração inicial do OpenGL
    // ------------------------------------------

    glClearColor(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    // ------------------------------------------
    // Callbacks do GLUT
    // ------------------------------------------

    glutDisplayFunc(desenhar);

    glutReshapeFunc(redimensionar);

    glutKeyboardFunc(teclado);

    // ------------------------------------------
    // Menu
    // ------------------------------------------

    criarMenu();

    // ------------------------------------------
    // Interface GLUI
    // ------------------------------------------

    criarInterface(caminho.c_str());

    // ------------------------------------------
    // Loop principal
    // ------------------------------------------

    glutMainLoop();

    return 0;
}
