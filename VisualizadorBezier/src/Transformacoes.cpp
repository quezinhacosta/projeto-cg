
#include "Transformacoes.h"

#include <GL/gl.h>

void rotacionar(double angulo)
{
    glMatrixMode(GL_MODELVIEW);
    glRotated(angulo, 0.0, 0.0, 1.0);
}

void transladar(double x, double y)
{
    glMatrixMode(GL_MODELVIEW);
    glTranslated(x, y, 0.0);
}
void escalar(double x, double y) 
{ 
        glMatrixMode(GL_MODELVIEW); 
        glScaled(x, y, 1.0); 
}
void refletirX() 
{ 
    glMatrixMode(GL_MODELVIEW); 
    glScaled(1.0, -1.0, 1.0); 
}
void refletirY() 
{ 
    glMatrixMode(GL_MODELVIEW); 
    glScaled(-1.0, 1.0, 1.0); 
}


void cisalharX(double fator) 
{ 
    glMatrixMode(GL_MODELVIEW); 
    const GLdouble matriz[] = { 
        1.0, 0.0, 0.0, 0.0, 
        fator, 1.0, 0.0, 0.0, 
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0 }; 
        
        glMultMatrixd(matriz); 
}

void cisalharY(double fator) 
{ 
    glMatrixMode(GL_MODELVIEW); 
    const GLdouble matriz[] = { 
        1.0, fator, 0.0, 0.0, 
        0.0, 1.0, 0.0, 0.0, 
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0 }; 
        
        glMultMatrixd(matriz); 
}

void resetarTransformacao()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
