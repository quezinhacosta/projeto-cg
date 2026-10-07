# Visualizador de Curvas de Bézier

Projeto da disciplina de Computação Gráfica para desenvolver um visualizador de curvas de Bézier com C++, OpenGL, GLUT e GLUI.

## Estado atual

O projeto C++ foi criado no Visual Studio e já compila em **Debug x64**. A aplicação abre uma janela OpenGL inicialmente com 800×600 pixels, fundo branco, eixo X verde e eixo Y azul.

A projeção ortográfica ajusta a proporção quando a janela é redimensionada. O código também implementa o encerramento pelas teclas `Q` e `ESC`.

**Ainda não implementado:** carregamento de arquivos `.obj`, desenho das curvas de Bézier, enquadramento automático do desenho, transformações 2D, menu do botão direito, interface GLUI e salvamento.

## Dependências

- Visual Studio com suporte a desenvolvimento para desktop com C++;
- OpenGL do Windows (`opengl32.lib` e `glu32.lib`);
- FreeGLUT para x64;
- GLUI: necessária para a versão final, mas ainda não integrada.

Neste computador, a FreeGLUT foi encontrada em uma instalação do vcpkg em `D:\vcpkg\installed\x64-windows`. **Esse caminho é local e deve ser ajustado em outros computadores.**

## Configuração feita no Visual Studio

O projeto foi configurado para a plataforma **x64**:

1. Em **Propriedades do projeto → C/C++ → Geral → Diretórios de Inclusão Adicionais**, foi adicionada a pasta `D:\vcpkg\installed\x64-windows\include`. Ela contém a subpasta `GL`, usada pelo código em `#include <GL/glut.h>`.
2. Em **Vinculador → Geral → Diretórios de Bibliotecas Adicionais**, foi adicionada a pasta `D:\vcpkg\installed\x64-windows\lib`.
3. Em **Vinculador → Entrada → Dependências Adicionais**, foram adicionadas `opengl32.lib`, `glu32.lib` e a biblioteca `.lib` correspondente à FreeGLUT instalada.
4. Para a execução, `freeglut.dll`, encontrada em `D:\vcpkg\installed\x64-windows\bin`, foi copiada para a mesma pasta que contém `VisualizadorBezier.exe`.

> A DLL foi copiada manualmente neste estágio. Essa cópia precisará ser repetida se o executável passar a ser gerado em outra pasta, por exemplo, ao mudar de Debug para Release.

## Como executar

1. Abrir a solução no Visual Studio.
2. Selecionar **Debug** e **x64**.
3. Conferir os caminhos da FreeGLUT nas propriedades do projeto e a presença de `freeglut.dll` ao lado do executável.
4. Compilar e executar com `Ctrl+F5`.

Resultado esperado: janela branca com eixo X verde e eixo Y azul.

## Próxima etapa

Ler de um arquivo `.obj` quatro pontos de controle e desenhar uma curva de Bézier cúbica. Depois, ampliar a leitura para curvas concatenadas e calcular o enquadramento automático da figura.



visando rodar o codigo no linux, basta
```
cd projeto-cg/VisualizadorBezier
```


rode isto para compilar todos os aquivos com seus .h
```
g++ src/main.cpp \
          src/ArquivoOBJ.cpp \
          src/Bezier.cpp \
          src/Menu.cpp \
          src/Visualizacao.cpp \
          src/Transformacoes.cpp \
          -Iinclude \
          -I/usr/local/include \
          -L/usr/local/lib \
          -lglui \
          -lglut \
          -lGLU \
          -lGL \
          -o curvas
```