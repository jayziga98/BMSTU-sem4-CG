#include "circle.h"
#include "vector.h"

void init(circle_t &c, const double x, const double y, const double r)
{
    c.x = x;
    c.y = y;
    c.r = r;
}

bool contains(const circle_t &c, const point_t &p)
{
    vector_t v_op;
    init(v_op, c.x, c.y, p.x, p.y);
    double len = length_sqr(v_op);

    return len <= c.r * c.r;
}
