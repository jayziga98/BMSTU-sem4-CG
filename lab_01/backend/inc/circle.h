#ifndef CIRCLE_H
#define CIRCLE_H

#include "point.h"
#include "vector.h"

typedef struct {
    double x;
    double y;
    double r;
} circle_t;

void init(circle_t &c, const double x, const double y, const double r);
bool contains(const circle_t &c, const point_t &p);
double fit(const circle_t &c, const vector_t &v1, const vector_t &v2);

#endif // CIRCLE_H
