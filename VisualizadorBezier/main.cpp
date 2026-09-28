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

// Controle de visualização
bool exibirPoligono = false;
double esquerda = -100.0;
double direita  =  100.0;
double baixo    = -100.0;
double cima     =  100.0;

// GLUI: Identificadores da interface
int janelaVisualizacao = 0;
GLUI_EditText* campoArquivo = nullptr;


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

        // Linhas que indicam vértices com coordenadas (x, y)
        if (tipo == "v") {
            Ponto ponto;
            if (leitor >> ponto.x >> ponto.y) {
                pontos.push_back(ponto);
            }
        }
        // Comentários (#) e linhas vazias são ignorados
    }

    if (pontos.size() != 4) {
        std::cerr << "Este primeiro teste precisa de exatamente 4 pontos; "
                  << "o arquivo contem " << pontos.size() << ".\n";
        return false;
    }

    // Só substitui a curva após a validação completa do arquivo
    pontosControle = pontos;
    std::cout << "Arquivo carregado: " << caminho << '\n';
    return true;
}

Ponto calcularBezier(double t) {
    const double u = 1.0 - t;

    // Polinômios de Bernstein de grau 3
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

void desenharEixos() {
    glLineWidth(2.0f);

    // Eixo X: verde
    glColor3f(0.0f, 0.7f, 0.0f);
    glBegin(GL_LINES);
    glVertex2d(esquerda, 0.0);
    glVertex2d(direita, 0.0);
    glEnd();

    // Eixo Y: azul
    glColor3f(0.0f, 0.2f, 1.0f);
    glBegin(GL_LINES);
    glVertex2d(0.0, baixo);
    glVertex2d(0.0, cima);
    glEnd();
}

void desenharPoligonoDeControle() {
    if (pontosControle.empty()) return;

    // Arestas do polígono (cinza escuro)
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& pt : pontosControle) {
        glVertex2d(pt.x, pt.y);
    }
    glEnd();

    // Vértices de controle (pontos pretos destacados)
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for (const auto& pt : pontosControle) {
        glVertex2d(pt.x, pt.y);
    }
    glEnd();
}

void desenhar() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha os eixos principais X e Y
    desenharEixos();

    if (!pontosControle.empty()) {
        // Opção do menu: desenha o polígono antes da curva se estiver ativo
        if (exibirPoligono) {
            desenharPoligonoDeControle();
        }

        // Desenho da curva de Bézier (vermelha)
        glColor3f(0.9f, 0.1f, 0.1f);
        glLineWidth(2.5f);
        glBegin(GL_LINE_STRIP);

        const int amostras = 100;
        for (int i = 0; i <= amostras; ++i) {
            const double t = static_cast<double>(i) / amostras;
            const Ponto ponto = calcularBezier(t);
            glVertex2d(ponto.x, ponto.y);
        }
        glEnd();
    }

    glutSwapBuffers();
}

void redimensionar(int largura, int altura) {
    largura = std::max(largura, 1);
    altura  = std::max(altura, 1);

    glViewport(0, 0, largura, altura);

    // Ajusta a escala incluindo a origem para manter ambos os eixos visíveis
    double minX = 0.0, maxX = 0.0;
    double minY = 0.0, maxY = 0.0;

    for (const Ponto& ponto : pontosControle) {
        minX = std::min(minX, ponto.x);
        maxX = std::max(maxX, ponto.x);
        minY = std::min(minY, ponto.y);
        maxY = std::max(maxY, ponto.y);
    }

    const double larguraBase = std::max(maxX - minX, 20.0);
    const double alturaBase  = std::max(maxY - minY, 20.0);

    const double centroX = (minX + maxX) / 2.0;
    const double centroY = (minY + maxY) / 2.0;

    double larguraVisivel = larguraBase * 1.2;
    double alturaVisivel  = alturaBase * 1.2;

    const double proporcaoJanela = static_cast<double>(largura) / altura;

    if (larguraVisivel / alturaVisivel < proporcaoJanela) {
        larguraVisivel = alturaVisivel * proporcaoJanela;
    } else {
        alturaVisivel = larguraVisivel / proporcaoJanela;
    }

    esquerda = centroX - larguraVisivel / 2.0;
    direita  = centroX + larguraVisivel / 2.0;
    baixo    = centroY - alturaVisivel / 2.0;
    cima     = centroY + alturaVisivel / 2.0;

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

void menuCallback(int opcao) {
    switch (opcao) {
        case 1:
            exibirPoligono = true;  // a) Exibir curva e polígono de controle
            break;
        case 2:
            exibirPoligono = false; // b) Exibir apenas a curva
            break;
    }
    glutPostRedisplay();
}

void criarMenu() {
    glutCreateMenu(menuCallback);
    glutAddMenuEntry("Exibir curva e poligono de controle", 1);
    glutAddMenuEntry("Exibir apenas a curva", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void carregarPelaInterface(int) {
    if (campoArquivo == nullptr) return;

    const std::string caminho = campoArquivo->get_text();
    if (caminho.empty()) {
        std::cerr << "Informe o caminho de um arquivo .obj.\n";
        return;
    }

    if (!carregarObj(caminho)) {
        return;
    }

    glutSetWindow(janelaVisualizacao);
    redimensionar(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    std::cout << "Curva atualizada pela interface.\n";
}

void botaoSairCallback(int) {
    std::exit(0);
}

void botaoSairCallback(int) {
    std::exit(0);
}

// ==========================================
// Função Principal
// ==========================================
int main(int argc, char** argv) {
    const std::string caminho = (argc > 1) ? argv[1] : "desenhos/teste.obj";

    if (!carregarObj(caminho)) {
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);

    janelaVisualizacao = glutCreateWindow("Visualizador de Curvas de Bezier");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(desenhar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);

    // Registra o menu do botão direito do mouse
    criarMenu();

    // Configuração da interface GLUI
    GLUI* interface = GLUI_Master.create_glui("Controles");
    campoArquivo = interface->add_edittext("Arquivo OBJ:");
    campoArquivo->set_text(caminho.c_str());

    interface->add_button("Carregar", 0, carregarPelaInterface);
    interface->add_button("Sair", 0, botaoSairCallback);
    interface->set_main_gfx_window(janelaVisualizacao);

    glutMainLoop();

    return 0;
}