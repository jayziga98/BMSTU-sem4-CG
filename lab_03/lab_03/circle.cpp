#include "circle.h"
#include <math.h>
#include "pixel.h"

template<class T>
static int sign(T num)
{
    if (num > 0)
        return 1;
    if (num < 0)
        return -1;
    return 0;
}

QGraphicsItem *circle(QRectF &circle, params_t &params, int *step)
{
    if (step)
        *step = 0;

    QGraphicsItem *item = nullptr;

    switch (params.type) {
    case CANONICAL:
        item = circle_canonical(circle, params.color);
        break;
    case PARAMETRIC:
        item = circle_param(circle, params.color);
        break;
    case BRESENHAM:
        item = circle_bresenham(circle, params.color);
        break;
    case MID_POINT:
        item = circle_midpoint(circle, params.color);
        break;
    case BIBLIO:
        item = circle_biblio(circle, params.color);
        break;
    }

    return item;
}

QGraphicsItem *circle_spectrum(QRectF &c, params_t &params, double step, int amount)
{
    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    for (int i = 0; i < amount; i++)
    {
        item->addToGroup(circle(c, params, nullptr));
        c.setX(c.x() - step);
        c.setY(c.y() - step);
        c.setWidth(c.width() + step);
        c.setHeight(c.height() + step);
    }

    return item;
}

QGraphicsItem *reflected(QPointF &center, QPointF &cur, QColor color)
{
    QGraphicsItemGroup *item = new QGraphicsItemGroup();
    item->addToGroup(pixel(QPointF(center.x() + cur.x(), center.y() + cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() + cur.x(), center.y() - cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.x(), center.y() + cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.x(), center.y() - cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() + cur.y(), center.y() + cur.x()), color));
    item->addToGroup(pixel(QPointF(center.x() + cur.y(), center.y() - cur.x()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.y(), center.y() + cur.x()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.y(), center.y() - cur.x()), color));

    return item;
}

QGraphicsItem *circle_biblio(QRectF &circle, QColor color)
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(circle);
    item->setPen(QPen(color));

    return item;
}

QGraphicsItem *circle_canonical(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    int limit = round(radius / sqrt(2));
    int radiusSqr = radius * radius;
    for (int x = 0; x <= limit; x++)
    {
        int y = round(sqrt(radiusSqr - x * x));
        QPointF c = circle.center();
        QPointF cur = QPointF(x, y);
        item->addToGroup(reflected(c, cur, color));
    }

    return item;
}

QGraphicsItem *circle_param(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();


    double step = 1 / (double)radius;
    for (double t = 0; t <= M_PI / 4; t += step)
    {
        int x = round(radius * cos(t));
        int y = round(radius * sin(t));
        QPointF c = circle.center();
        QPointF cur = QPointF(x, y);
        item->addToGroup(reflected(c, cur, color));
    }

    return item;
}

QGraphicsItem *circle_bresenham(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double d = 2 * (1 - radius);
    int limit = round(radius/sqrt(2));
    int x = 0;
    int y = radius;
    while (y >= limit)
    {
        QPointF c = circle.center();
        QPointF cur = QPointF(x, y);
        item->addToGroup(reflected(c, cur, color));

        if (d < 0)
        {
            x++;
            int d1 = d + y * 2- 1;
            if (d1 + d < 0)
                d += 2 * x + 1;
            else
            {
                y--;
                d += 2 * (x - y + 1);
            }
        }
        else if (d > 0)
        {
            y--;
            int d2 = d - 2 * x - 1;
            if (d2 + d < 0)
            {
                x++;
                d += 2 * (x - y + 1);
            }
        }
        else
        {
            x++;
            y--;
            d += 2 * (x - y + 1);
        }
    }

    return item;
}

QGraphicsItem *circle_midpoint(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double d = 1.25 - radius;
    int x = 0;
    int y = radius;
    while (x <= y)
    {
        QPointF c = circle.center();
        QPointF cur = QPointF(x, y);
        item->addToGroup(reflected(c, cur, color));

        d += 2 * x + 1;
        if (d < 0)
            x++;
        else
        {
            x++;
            y--;
            d += -2 * y;
        }
    }

    return item;
}
