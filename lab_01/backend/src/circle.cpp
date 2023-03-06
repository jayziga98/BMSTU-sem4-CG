#include "circle.h"
#include "vector.h"

#include <QPoint>
#include <qdebug.h>

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

double fit(const circle_t &c, const vector_t &v1, const vector_t &v2)
{
    vector_t v;
    init(v, v1.x, v1.y, v2.x, v2.y);
    double scale = c.r * 2 / length(v);

    return scale;
}
