
// Estrutura que representa um ponto no espaco 2D (com z para conveniencia no OpenGL)
struct Ponto2D {
    float x;
    float y;

    Ponto2D(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
};