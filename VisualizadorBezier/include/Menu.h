#ifndef MENU_H
#define MENU_H

extern int janelaVisualizacao;
extern bool exibirTransformada;

void criarMenu();
void criarInterface(const char* caminho);
void teclado(unsigned char tecla, int x, int y);

// Controle das transformações acumuladas
void aplicarTransformacoesAcumuladas();
void resetarTransformacoes();



#endif
