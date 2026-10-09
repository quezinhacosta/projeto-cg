# Visualizador de Curvas de Bézier

Projeto da disciplina de Computação Gráfica para desenvolver um visualizador de curvas de Bézier com C++, OpenGL, GLUT e GLUI.

## Estado atual

O projeto C++ foi criado no Visual Studio e já compila em **Debug x64**. A aplicação abre uma janela OpenGL inicialmente com 800×600 pixels, fundo branco, eixo X verde e eixo Y azul.

A projeção ortográfica ajusta a proporção quando a janela é redimensionada. O código também implementa o encerramento pelas teclas `Q` e `ESC`.

O visualizador lê os pontos de controle de arquivos `.obj`, desenha vários trechos cúbicos concatenados e permite separar contornos independentes com registros `o` ou `g`. O arquivo `desenhos/rosa.obj` é um exemplo com mais de 300 pontos de controle.

## Dependências

- Visual Studio com suporte a desenvolvimento para desktop com C++;
- OpenGL do Windows (`opengl32.lib` e `glu32.lib`);
- FreeGLUT para x64;
- GLUI (`glui.lib`), usada pela interface de controles.

Neste computador, a FreeGLUT foi encontrada em uma instalação do vcpkg em `D:\vcpkg\installed\x64-windows`. **Esse caminho é local e deve ser ajustado em outros computadores.**

## Configuração feita no Visual Studio

O projeto foi configurado para a plataforma **x64**:

1. Em **Propriedades do projeto → C/C++ → Geral → Diretórios de Inclusão Adicionais**, foi adicionada a pasta `D:\vcpkg\installed\x64-windows\include`. Ela contém a subpasta `GL`, usada pelo código em `#include <GL/glut.h>`.
2. Em **Vinculador → Geral → Diretórios de Bibliotecas Adicionais**, foi adicionada a pasta `D:\vcpkg\installed\x64-windows\lib`.
3. Em **Vinculador → Entrada → Dependências Adicionais**, foram adicionadas `opengl32.lib`, `glu32.lib`, `freeglut.lib` e `glui.lib`.
4. Para a execução, `freeglut.dll`, encontrada em `D:\vcpkg\installed\x64-windows\bin`, foi copiada para a mesma pasta que contém `VisualizadorBezier.exe`.

> A DLL foi copiada manualmente neste estágio. Essa cópia precisará ser repetida se o executável passar a ser gerado em outra pasta, por exemplo, ao mudar de Debug para Release.

## Como executar

1. Abrir a solução no Visual Studio.
2. Selecionar **Debug** e **x64**.
3. Conferir os caminhos da FreeGLUT nas propriedades do projeto e a presença de `freeglut.dll` ao lado do executável.
4. Compilar e executar com `Ctrl+F5`.

Resultado esperado: janela branca com a rosa, os eixos X/Y e os controles da interface.

## Formato dos arquivos de curva

Use uma linha `v x y` para cada ponto. Registros OBJ `o` ou `g` separam contornos independentes. Um contorno com pelo menos quatro pontos e contagem `3k + 1` (4, 7, 10, ...) é desenhado como curvas cúbicas de Bézier; os trechos compartilham pontos extremos (1–4, 4–7 e assim por diante). Qualquer outra quantidade de pontos é aceita e desenhada como polilinha, conectando os vértices na ordem do arquivo. Para fechar um contorno, repita como último ponto o primeiro.

Arquivos `.obj` são usados aqui como listas de vértices 2D (`v x y`), não como malhas 3D: faces e índices OBJ são ignorados. O aviso para menos de 300 pontos é informativo e não impede o carregamento.

## Executar no Linux

No diretório do projeto:
```
cd VisualizadorBezier
```

Compile os módulos:
```
g++ -std=c++17 src/main.cpp \
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

Execute a rosa de exemplo (ou passe outro OBJ como argumento):
```
./curvas desenhos/rosa.obj
```