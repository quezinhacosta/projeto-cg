#include "Menu.h"

#include "ArquivoOBJ.h"
#include "Visualizacao.h"
#include "Ponto.h"
#include "Transformacoes.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <GL/glui.h>

int janelaVisualizacao = 0;

GLUI_EditText* campoArquivo = nullptr;
GLUI_EditText* campoSalvar = nullptr;

GLUI_EditText* campoRotacao = nullptr;

GLUI_EditText* campoTranslacaoX = nullptr;
GLUI_EditText* campoTranslacaoY = nullptr;

GLUI_EditText* campoEscalaX = nullptr;
GLUI_EditText* campoEscalaY = nullptr;

GLUI_EditText* campoCisalhamentoX = nullptr;
GLUI_EditText* campoCisalhamentoY = nullptr;

bool exibirPoligono = false;
bool exibirTransformada = true;

enum TipoTransformacao
{
    ROTACAO,
    TRANSLACAO,
    ESCALA,
    REFLEXAO_X,
    REFLEXAO_Y,
    CISALHAMENTO_X,
    CISALHAMENTO_Y
};

struct Transformacao
{
    TipoTransformacao tipo;

    double valor1;
    double valor2;
};

std::vector<Transformacao> transformacoes;

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

    resetarTransformacoes();

    glutSetWindow(janelaVisualizacao);

    redimensionar(
        glutGet(GLUT_WINDOW_WIDTH),
        glutGet(GLUT_WINDOW_HEIGHT)
    );

    std::cout
        << "Curva atualizada pela interface.\n";
}

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


void botaoSairCallback(int)
{
    std::exit(0);
}



double lerValor(GLUI_EditText* campo)
{
    if (campo == nullptr) {
        return 0.0;
    }

    const std::string texto =
        campo->get_text();

    try {
        return std::stod(texto);
    }
    catch (...) {
        return 0.0;
    }
}

void botaoRotacao(int)
{
    if (campoRotacao == nullptr) {
        return;
    }

    const double angulo =
        lerValor(campoRotacao);

    std::cout
        << "VALOR LIDO PELO GLUI: "
        << angulo
        << "\n";

    Transformacao transformacao;

    transformacao.tipo = ROTACAO;
    transformacao.valor1 = angulo;
    transformacao.valor2 = 0.0;

    transformacoes.push_back(transformacao);

    glutPostRedisplay();
}

void botaoTranslacao(int)
{
    if (campoTranslacaoX == nullptr ||
        campoTranslacaoY == nullptr) {

        return;
    }

    const double x =
        lerValor(campoTranslacaoX);

    const double y =
        lerValor(campoTranslacaoY);

    Transformacao transformacao;

    transformacao.tipo = TRANSLACAO;
    transformacao.valor1 = x;
    transformacao.valor2 = y;

    transformacoes.push_back(transformacao);

    std::cout
        << "Translacao adicionada: ("
        << x << ", "
        << y << ")\n";

    glutPostRedisplay();
}

void botaoEscala(int)
{
    if (campoEscalaX == nullptr ||
        campoEscalaY == nullptr) {

        return;
    }

    const double x =
        lerValor(campoEscalaX);

    const double y =
        lerValor(campoEscalaY);

    Transformacao transformacao;

    transformacao.tipo = ESCALA;
    transformacao.valor1 = x;
    transformacao.valor2 = y;

    transformacoes.push_back(transformacao);

    std::cout
        << "Escala adicionada: ("
        << x << ", "
        << y << ")\n";

    glutPostRedisplay();
}

void botaoReflexaoX(int)
{
    Transformacao transformacao;

    transformacao.tipo = REFLEXAO_X;
    transformacao.valor1 = 0.0;
    transformacao.valor2 = 0.0;

    transformacoes.push_back(transformacao);

    std::cout
        << "Reflexao em X adicionada.\n";

    glutPostRedisplay();
}

void botaoReflexaoY(int)
{
    Transformacao transformacao;

    transformacao.tipo = REFLEXAO_Y;
    transformacao.valor1 = 0.0;
    transformacao.valor2 = 0.0;

    transformacoes.push_back(transformacao);

    std::cout
        << "Reflexao em Y adicionada.\n";

    glutPostRedisplay();
}

void botaoCisalhamentoX(int)
{
    if (campoCisalhamentoX == nullptr) {
        return;
    }

    const double fator =
        lerValor(campoCisalhamentoX);

    Transformacao transformacao;

    transformacao.tipo = CISALHAMENTO_X;
    transformacao.valor1 = fator;
    transformacao.valor2 = 0.0;

    transformacoes.push_back(transformacao);

    std::cout
        << "Cisalhamento em X adicionado: "
        << fator
        << "\n";

    glutPostRedisplay();
}

void botaoCisalhamentoY(int)
{
    if (campoCisalhamentoY == nullptr) {
        return;
    }

    const double fator =
        lerValor(campoCisalhamentoY);

    Transformacao transformacao;

    transformacao.tipo = CISALHAMENTO_Y;
    transformacao.valor1 = fator;
    transformacao.valor2 = 0.0;

    transformacoes.push_back(transformacao);

    std::cout
        << "Cisalhamento em Y adicionado: "
        << fator
        << "\n";

    glutPostRedisplay();
}

void aplicarTransformacoesAcumuladas()
{
    for (const Transformacao& transformacao : transformacoes) {

        switch (transformacao.tipo) {

            case ROTACAO:
                rotacionar(transformacao.valor1);
                break;

            case TRANSLACAO:
                transladar(
                    transformacao.valor1,
                    transformacao.valor2
                );
                break;

            case ESCALA:
                escalar(
                    transformacao.valor1,
                    transformacao.valor2
                );
                break;

            case REFLEXAO_X:
                refletirX();
                break;

            case REFLEXAO_Y:
                refletirY();
                break;

            case CISALHAMENTO_X:
                cisalharX(transformacao.valor1);
                break;

            case CISALHAMENTO_Y:
                cisalharY(transformacao.valor1);
                break;
        }
    }
}

void resetarTransformacoes()
{
    transformacoes.clear();

    std::cout
        << "Transformacoes resetadas.\n";

    glutPostRedisplay();
}


void botaoDesenhoOriginal(int)
{
    exibirTransformada = false;

    glutPostRedisplay();
}

void botaoDesenhoOriginalETransformado(int)
{
    exibirTransformada = true;

    glutPostRedisplay();
}

void teclado(unsigned char tecla, int, int)
{
    if (tecla == 'q' ||
        tecla == 'Q' ||
        tecla == 27) {

        std::exit(0);
    }
}

void botaoResetarCallback(int)
{
    resetarTransformacoes();
}

void criarInterface(const char* caminho)
{
    GLUI* interface =
        GLUI_Master.create_glui("Controles");


    interface->add_statictext(
        "Arquivo"
    );

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

    interface->add_statictext(
        "Rotacao"
    );

    campoRotacao =
        interface->add_edittext(
            "Angulo:"
        );

    campoRotacao->set_float_val(45.0);

    interface->add_button(
        "Rotacionar",
        0,
        botaoRotacao
    );

    interface->add_separator();

    interface->add_statictext(
        "Translacao"
    );

    campoTranslacaoX =
        interface->add_edittext(
            "X:"
        );

    campoTranslacaoX->set_float_val(0.0);

    campoTranslacaoY =
        interface->add_edittext(
            "Y:"
        );

    campoTranslacaoY->set_float_val(0.0);

    interface->add_button(
        "Transladar",
        0,
        botaoTranslacao
    );

    interface->add_separator();

    interface->add_statictext(
        "Escala"
    );

    campoEscalaX =
        interface->add_edittext(
            "X:"
        );

    campoEscalaX->set_float_val(1.0);

    campoEscalaY =
        interface->add_edittext(
            "Y:"
        );

    campoEscalaY->set_float_val(1.0);

    interface->add_button(
        "Escalar",
        0,
        botaoEscala
    );

    interface->add_separator();

    interface->add_statictext(
        "Reflexao"
    );

    interface->add_button(
        "Refletir em X",
        0,
        botaoReflexaoX
    );

    interface->add_button(
        "Refletir em Y",
        0,
        botaoReflexaoY
    );

    interface->add_separator();

    interface->add_statictext(
        "Cisalhamento"
    );

    campoCisalhamentoX =
        interface->add_edittext(
            "Fator X:"
        );

    campoCisalhamentoX->set_float_val(0.0);

    interface->add_button(
        "Cisalhar X",
        0,
        botaoCisalhamentoX
    );

    campoCisalhamentoY =
        interface->add_edittext(
            "Fator Y:"
        );

    campoCisalhamentoY->set_float_val(0.0);

    interface->add_button(
        "Cisalhar Y",
        0,
        botaoCisalhamentoY
    );

    interface->add_separator();

    interface->add_button(
        "Resetar transformacoes",
        0,
        botaoResetarCallback
    );

    interface->add_separator();


        interface->add_statictext(
        "Visualizacao"
    );

    interface->add_button(
        "Desenho original",
        0,
        botaoDesenhoOriginal
    );

    interface->add_button(
        "Original + transformado",
        0,
        botaoDesenhoOriginalETransformado
    );

    interface->add_separator();

    interface->add_button(
        "Sair",
        0,
        botaoSairCallback
    );

    interface->set_main_gfx_window(
        janelaVisualizacao
    );
}
