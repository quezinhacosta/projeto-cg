#include "Menu.h"

#include "ArquivoOBJ.h"
#include "Visualizacao.h"
#include "Ponto.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <GL/glui.h>

// ==========================================
// Variáveis da interface
// ==========================================

int janelaVisualizacao = 0;

GLUI_EditText* campoArquivo = nullptr;
GLUI_EditText* campoSalvar = nullptr;

// ==========================================
// Controle do menu
// ==========================================

bool exibirPoligono = false;

// ==========================================
// Menu do botão direito
// ==========================================

void menuCallback(int opcao)
{
    switch (opcao) {

        case 1:
            exibirPoligono = true;
            break;

        case 2:
            exibirPoligono = false;
            break;
    }

    glutPostRedisplay();
}

void criarMenu()
{
    glutCreateMenu(menuCallback);

    glutAddMenuEntry(
        "Exibir curva e poligono de controle",
        1
    );

    glutAddMenuEntry(
        "Exibir apenas a curva",
        2
    );

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ==========================================
// Carregar pela interface
// ==========================================

void carregarPelaInterface(int)
{
    if (campoArquivo == nullptr) {
        return;
    }

    const std::string caminho =
        campoArquivo->get_text();

    if (caminho.empty()) {
        std::cerr
            << "Informe o caminho de um arquivo .obj.\n";
        return;
    }

    if (!carregarObj(caminho)) {
        return;
    }

    glutSetWindow(janelaVisualizacao);

    redimensionar(
        glutGet(GLUT_WINDOW_WIDTH),
        glutGet(GLUT_WINDOW_HEIGHT)
    );

    std::cout
        << "Curva atualizada pela interface.\n";
}

// ==========================================
// Salvar pela interface
// ==========================================

void salvarPelaInterface(int)
{
    if (campoSalvar == nullptr) {
        return;
    }

    const std::string caminho =
        campoSalvar->get_text();

    if (caminho.empty()) {
        std::cerr
            << "Informe um nome de arquivo valido para salvar.\n";
        return;
    }

    salvarObj(caminho);
}

// ==========================================
// Botão Sair
// ==========================================

void botaoSairCallback(int)
{
    std::exit(0);
}

// ==========================================
// Teclado
// ==========================================

void teclado(unsigned char tecla, int, int)
{
    if (tecla == 'q' ||
        tecla == 'Q' ||
        tecla == 27) {

        std::exit(0);
    }
}

// ==========================================
// Interface GLUI
// ==========================================

void criarInterface(const char* caminho)
{
    GLUI* interface =
        GLUI_Master.create_glui("Controles");

    // Campo para carregar OBJ
    campoArquivo =
        interface->add_edittext(
            "Arquivo OBJ:"
        );

    campoArquivo->set_text(caminho);

    interface->add_button(
        "Carregar",
        0,
        carregarPelaInterface
    );

    interface->add_separator();

    // Campo para salvar OBJ
    campoSalvar =
        interface->add_edittext(
            "Salvar como:"
        );

    campoSalvar->set_text(
        "desenhos/salvo.obj"
    );

    interface->add_button(
        "Salvar",
        0,
        salvarPelaInterface
    );

    interface->add_separator();

    // Botão Sair
    interface->add_button(
        "Sair",
        0,
        botaoSairCallback
    );

    interface->set_main_gfx_window(
        janelaVisualizacao
    );
}
