#ifndef VECTOR_H
#define VECTOR_H

#include "point.h"

typedef struct {
    double x;
    double y;
} vector_t;

void init(vector_t &v, const double x, const double y);
void init(vector_t &v, const double x1, const double y1, const double x2, const double y2);
double length(const vector_t &v);
double length_sqr(const vector_t &v);
double dot_prod(const vector_t &v1, const vector_t &v2);
double cross_prod(const vector_t &v1, const vector_t &v2);
int direction(const vector_t &a, const vector_t &b, const point_t &p);

#endif // VECTOR_H
