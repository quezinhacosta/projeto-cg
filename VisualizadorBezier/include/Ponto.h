#ifndef PONTO_H
#define PONTO_H

#include <vector>

struct Ponto {
    double x;
    double y;
};

using ContornoBezier = std::vector<Ponto>;
using FiguraBezier = std::vector<ContornoBezier>;

#endif
