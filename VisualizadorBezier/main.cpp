#include <cstdlib>
#include <GL/glut.h>

void desenhar() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // Eixo X: verde
    glColor3f(0.0f, 0.7f, 0.0f);
    glVertex2f(-100.0f, 0.0f);
    glVertex2f(100.0f, 0.0f);

    // Eixo Y: azul
    glColor3f(0.0f, 0.2f, 1.0f);
    glVertex2f(0.0f, -100.0f);
    glVertex2f(0.0f, 100.0f);

    glEnd();
    glutSwapBuffers();
}

void redimensionar(int largura, int altura) {
    if (altura == 0) altura = 1;

    glViewport(0, 0, largura, altura);

    const double proporcao = static_cast<double>(largura) / altura;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Mantém a mesma escala em X e Y ao mudar o formato da janela.
    if (proporcao >= 1.0) {
        gluOrtho2D(-100.0 * proporcao, 100.0 * proporcao,
            -100.0, 100.0);
    }
    else {
        gluOrtho2D(-100.0, 100.0,
            -100.0 / proporcao, 100.0 / proporcao);
    }

    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void teclado(unsigned char tecla, int, int) {
    if (tecla == 'q' || tecla == 'Q' || tecla == 27) {
        std::exit(0);
    }
}

int main(int argc, char** argv) {
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