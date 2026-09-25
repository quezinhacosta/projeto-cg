#include <iostream>
#include <vector>
#include <string>

#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glui.h>

#include "Ponto.h"
#include "Bezier.h"
#include "ArquivoOBJ.h"
#include "Transformacoes.h"

// Dimensoes iniciais da janela de exibicao (800x600)
int winWidth = 800;
int winHeight = 600;
int mainWindowId = 0;

// Estados das figuras para permitir visualizar a curva antes e apos transformacao
std::vector<Ponto2D> pontosOriginais;
std::vector<Ponto2D> pontosAnteriores;
std::vector<Ponto2D> pontosAtuais;
bool houveTransformacao = false;

// Controle de exibicao do poligono (Menu do botao direito)
bool exibirPoligono = true;

// Variaveis de controle ligadas a interface GLUI
char nomeArquivoCarregar[128] = "quezia.obj";
char nomeArquivoSalvar[128] = "quezia_transformado.obj";

float transX = 0.0f, transY = 0.0f;
float anguloRot = 0.0f;
float escalaX = 1.0f, escalaY = 1.0f;

// IDs de callbacks da GLUI
enum {
    CB_CARREGAR = 1,
    CB_SALVAR,
    CB_APLICAR_TRANSFORMACOES,
    CB_RESTAURAR_ORIGINAL,
    CB_SAIR
};

// Funcao de encerramento da aplicacao
void encerrarAplicacao() {
    exit(0);
}

// Desenha os eixos cartesianos X (Verde) e Y (Azul)
void desenharEixos() {
    glLineWidth(1.8f);
    glBegin(GL_LINES);

    // Eixo X - Verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-10000.0f, 0.0f);
    glVertex2f(10000.0f, 0.0f);

    // Eixo Y - Azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.0f, -10000.0f);
    glVertex2f(0.0f, 10000.0f);

    glEnd();
}

// Funcao principal de desenho do OpenGL
void display() {
    // Fundo branco
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Os eixos principais X e Y devem estar sempre presentes
    desenharEixos();

    // 1. Se ocorreu transformacao, exibe a curva na posicao anterior (em cinza escuro/roxo)
    if (houveTransformacao && !pontosAnteriores.empty()) {
        glColor3f(0.45f, 0.45f, 0.5f);
        desenharCurvasBezier(pontosAnteriores);
    }

    // 2. Exibe a figura na posicao atual com as cores pedidas (Rosa para o nome "Quezia")
    if (!pontosAtuais.empty()) {
        if (exibirPoligono) {
            desenharPoligonoDeControle(pontosAtuais);
        }
        // Rosa: RGB(1.0, 0.2, 0.6)
        glColor3f(1.0f, 0.2f, 0.6f);
        desenharCurvasBezier(pontosAtuais);
    }

    glutSwapBuffers();
}

// Mantem a proporcionalidade da figura na projecao ortografica 2D ao redimensionar
void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)(h > 0 ? h : 1);
    float baseRange = 150.0f;

    if (w <= h) {
        gluOrtho2D(-baseRange, baseRange, -baseRange / aspect, baseRange / aspect);
    }
    else {
        gluOrtho2D(-baseRange * aspect, baseRange * aspect, -baseRange, baseRange);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Atalhos de teclado: 'Q', 'q' e 'ESC' (ASCII 27) encerram a aplicacao
void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27) {
        encerrarAplicacao();
    }
}

// Callback do Menu GLUT (Botao direito do mouse)
void menuPopUp(int item) {
    switch (item) {
    case 1:
        exibirPoligono = true;
        break;
    case 2:
        exibirPoligono = false;
        break;
    }
    glutPostRedisplay();
}

void criarMenuContexto() {
    glutCreateMenu(menuPopUp);
    glutAddMenuEntry("a) Exibir a curva e o poligono de controle", 1);
    glutAddMenuEntry("b) Exibir apenas a curva", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Callback para acoes dos botoes da interface GLUI
void gluiCallback(int id) {
    switch (id) {
    case CB_CARREGAR:
        if (carregarOBJ(nomeArquivoCarregar, pontosOriginais)) {
            pontosAtuais = pontosOriginais;
            pontosAnteriores.clear();
            houveTransformacao = false;
        }
        break;

    case CB_SALVAR:
        salvarOBJ(nomeArquivoSalvar, pontosAtuais);
        break;

    case CB_APLICAR_TRANSFORMACOES: {
        if (pontosAtuais.empty()) break;

        // Guarda a figura anterior para exibir ambas simultaneamente
        pontosAnteriores = pontosAtuais;

        // Aplica transformacao sequencial composta: Escala -> Rotacao -> Translacao
        std::vector<Ponto2D> temp = pontosAtuais;
        temp = escalar(temp, escalaX, escalaY);
        temp = rotacionar(temp, anguloRot);
        temp = transladar(temp, transX, transY);

        pontosAtuais = temp;
        houveTransformacao = true;
        break;
    }

    case CB_RESTAURAR_ORIGINAL:
        pontosAtuais = pontosOriginais;
        pontosAnteriores.clear();
        houveTransformacao = false;
        break;

    case CB_SAIR:
        encerrarAplicacao();
        break;
    }

    glutSetWindow(mainWindowId);
    glutPostRedisplay();
}

// Montagem do painel de controle via GLUI
void setupGLUI() {
    GLUI* glui = GLUI_Master.create_glui_subwindow(mainWindowId, GLUI_SUBWINDOW_RIGHT);

    // Painel: Arquivos
    GLUI_Panel* painelArquivo = glui->add_panel("Arquivo .OBJ");
    glui->add_edittext_to_panel(painelArquivo, "Abrir:", GLUI_EDITTEXT_TEXT, nomeArquivoCarregar);
    glui->add_button_to_panel(painelArquivo, "Carregar", CB_CARREGAR, gluiCallback);
    glui->add_separator_to_panel(painelArquivo);
    glui->add_edittext_to_panel(painelArquivo, "Salvar:", GLUI_EDITTEXT_TEXT, nomeArquivoSalvar);
    glui->add_button_to_panel(painelArquivo, "Salvar Figura", CB_SALVAR, gluiCallback);

    // Painel: Transformacoes 2D
    GLUI_Panel* painelTrans = glui->add_panel("Transformacoes 2D");

    // Translacao
    GLUI_Spinner* spTX = glui->add_spinner_to_panel(painelTrans, "Trans X:", GLUI_SPINNER_FLOAT, &transX);
    spTX->set_float_limits(-200.0f, 200.0f);
    GLUI_Spinner* spTY = glui->add_spinner_to_panel(painelTrans, "Trans Y:", GLUI_SPINNER_FLOAT, &transY);
    spTY->set_float_limits(-200.0f, 200.0f);

    // Rotacao
    GLUI_Spinner* spRot = glui->add_spinner_to_panel(painelTrans, "Rotacao (graus):", GLUI_SPINNER_FLOAT, &anguloRot);
    spRot->set_float_limits(-360.0f, 360.0f);

    // Escala
    GLUI_Spinner* spSX = glui->add_spinner_to_panel(painelTrans, "Escala X:", GLUI_SPINNER_FLOAT, &escalaX);
    spSX->set_float_limits(0.1f, 5.0f);
    GLUI_Spinner* spSY = glui->add_spinner_to_panel(painelTrans, "Escala Y:", GLUI_SPINNER_FLOAT, &escalaY);
    spSY->set_float_limits(0.1f, 5.0f);

    glui->add_button_to_panel(painelTrans, "Aplicar Sequencia", CB_APLICAR_TRANSFORMACOES, gluiCallback);
    glui->add_button_to_panel(painelTrans, "Restaurar Original", CB_RESTAURAR_ORIGINAL, gluiCallback);

    // Painel: Encerramento
    glui->add_separator();
    glui->add_button("Sair (Q / ESC)", CB_SAIR, gluiCallback);

    glui->set_main_gfx_window(mainWindowId);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);

    mainWindowId = glutCreateWindow("Visualizador de Curvas de Bezier 2D");

    // Registra callbacks da GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Cria o menu do botao direito
    criarMenuContexto();

    // Carrega o arquivo padrao com o nome Quezia
    if (carregarOBJ(nomeArquivoCarregar, pontosOriginais)) {
        pontosAtuais = pontosOriginais;
    }

    // Inicializa a interface GLUI
    setupGLUI();

    glutMainLoop();
    return 0;
}