#include "vector.h"

void init(vector_t &v, double x, double y)
{
    v.x = x;
    v.y = y;
}

void init(vector_t &v, double x1, double y1, double x2, double y2)
{
    v.x = x2 - x1;
    v.y = y2 - y1;
}

double dot_prod(vector_t &v1, vector_t &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

double cross_prod(vector_t &v1, vector_t &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}


