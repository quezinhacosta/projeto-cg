
#ifndef BEZIER_H
#define BEZIER_H

#include "Ponto.h"

Ponto calcularBezier(
    const Ponto& p0,
    const Ponto& p1,
    const Ponto& p2,
    const Ponto& p3,
    double t
);

#endif
