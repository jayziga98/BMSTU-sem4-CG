#ifndef POINT_H
#define POINT_H

#include <cstdio>
#include <math.h>

const double pi = acos(-1);

typedef struct {
    double x;
    double y;
    double z;
} point_t;

point_t point_init(double x, double y, double z);
point_t point_scan(FILE *stream, bool &ok);
void point_move(point_t &point, double dx, double dy);
void point_rotate(point_t &point, double deg);
void point_scale(point_t &point, double kx, double ky);

#endif // POINT_H
