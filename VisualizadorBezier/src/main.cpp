#include <iostream>
#include <string>

#include <GL/glut.h>

#include "Ponto.h"
#include "ArquivoOBJ.h"
#include "Bezier.h"
#include "Menu.h"
#include "Visualizacao.h"
#include "Transformacoes.h"


std::vector<Ponto> pontosControle;


double esquerda = -100.0;
double direita  = 100.0;
double baixo    = -100.0;
double cima     = 100.0;

int main(int argc, char** argv)
{

    const std::string caminho =
        (argc > 1)
            ? argv[1]
            : "desenhos/mario.obj";


    if (!carregarObj(caminho)) {
        return 1;
    }

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGB
    );

    glutInitWindowSize(
        800,
        600
    );

    janelaVisualizacao =
        glutCreateWindow(
            "Visualizador de Curvas de Bezier"
        );

    glClearColor(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    glutDisplayFunc(desenhar);

    glutReshapeFunc(redimensionar);

    glutKeyboardFunc(teclado);

    criarMenu();

    criarInterface(caminho.c_str());

    glutMainLoop();

    return 0;
}
