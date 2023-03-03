#ifndef POINT_H
#define POINT_H

typedef struct {
    double x;
    double y;
} point_t;

void init(point_t &p, double x, double y);

#endif // POINT_H
