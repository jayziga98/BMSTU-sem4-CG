#include "circle.h"

template<class T>
static int sign(T num)
{
    if (num > 0)
        return 1;
    if (num < 0)
        return -1;
    return 0;
}

QGraphicsItem *circle(QRectF &circle, circle_params_t &params, int *step)
{
    if (step)
        *step = 0;

    QGraphicsItem *item = nullptr;

    switch (params.type) {
    case CANONICAL:
        break;
    case PARAMETRIC:
        break;
    case BRESENHAM:
        break;
    case MID_POINT:
        break;
    case BIBLIO:
        item = circle_biblio(circle, params.color);
        break;
    }

    return item;
}

QGraphicsItem *circle_biblio(QRectF &circle, QColor color)
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(circle);
    item->setPen(QPen(color));

    return item;
}
