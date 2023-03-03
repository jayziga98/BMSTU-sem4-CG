#include "vector.h"
#include <math.h>

void init(vector_t &v, const double x, const double y)
{
    v.x = x;
    v.y = y;
}

void init(vector_t &v, const double x1, const double y1, const double x2, const double y2)
{
    v.x = x2 - x1;
    v.y = y2 - y1;
}

double length(const vector_t &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

double length_sqr(const vector_t &v)
{
    return v.x * v.x + v.y * v.y;
}

double dot_prod(const vector_t &v1, const vector_t &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

double cross_prod(const vector_t &v1, const vector_t &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

int direction(const vector_t &a, const vector_t &b, const point_t &p)
{
    vector_t ap;
    init(ap, a.x, a.y, p.x, p.y);

    vector_t ab;
    init(ab, a.x, a.y, b.x, b.y);

    return cross_prod(ap, ab);
}
