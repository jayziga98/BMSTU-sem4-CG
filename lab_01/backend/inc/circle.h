#ifndef CIRCLE_H
#define CIRCLE_H

#include "point.h"

typedef struct {
    double x;
    double y;
    double r;
} circle_t;

void init(circle_t &c, const double x, const double y, const double r);
bool contains(const circle_t &c, const point_t &p);

#endif // CIRCLE_H
