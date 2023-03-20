#include "point.h"
#include <cstdlib>

point_t point_init(double x, double y, double z)
{
    point_t point;
    point.x = x;
    point.y = y;
    point.z = z;
    return point;
}

point_t point_scan(FILE *stream, bool &ok)
{
    double x, y;
    ok = (fscanf(stream, "%lf%lf", &x, &y) == 2);

    return point_init(x, y, 0);
}

void point_move(point_t &point, double dx, double dy)
{
    point.x += dx;
    point.y += dy;
}

void point_rotate(point_t &point, double deg)
{
    double rad = deg * pi / 180;

    double new_x = point.x * cos(rad) - point.y * sin(rad);
    double new_y = point.y * cos(rad) + point.x * sin(rad);

    point.x = new_x;
    point.y = new_y;
}

void point_scale(point_t &point, double kx, double ky)
{
    point.x *= kx;
    point.y *= ky;
}
