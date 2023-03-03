#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double x;
    double y;
} vector_t;

void init(vector_t &v, double x1, double y1, double x2, double y2);

#endif // VECTOR_H
