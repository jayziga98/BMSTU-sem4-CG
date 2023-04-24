#ifndef ALGOTYPES_H
#define ALGOTYPES_H

enum algoType {
    CANONICAL,
    PARAMETRIC,
    BRESENHAM,
    MID_POINT,
    BIBLIO
};

typedef struct {
    algoType type;
    QColor color;
} params_t;

#endif // ALGOTYPES_H
