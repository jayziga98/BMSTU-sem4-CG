#include "ellipse.h"
#include "QtGui/qpen.h"
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

QGraphicsItem *ellipse(QRectF &circle, params_t &params, int *step)
{
    if (step)
        *step = 0;

    QGraphicsItem *item = nullptr;

    switch (params.type) {
    case CANONICAL:
        item = ellipse_canonical(circle, params.color);
        break;
    case PARAMETRIC:
        item = ellipse_param(circle, params.color);
        break;
    case BRESENHAM:
        item = ellipse_bresenham(circle, params.color);
        break;
    case MID_POINT:
        item = ellipse_midpoint(circle, params.color);
        break;
    case BIBLIO:
        item = ellipse_biblio(circle, params.color);
        break;
    }

    return item;
}

QGraphicsItem *ellipse_spectrum(QRectF &circle, params_t &params, double step, int amount)
{
    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    for (int i = 0; i < amount; i++)
    {
        auto circle2 = circle;
        item->addToGroup(ellipse(circle2, params, nullptr));
        circle.setX(circle.x() - step);
        circle.setY(circle.y() - step);
        circle.setWidth(circle.width() + step);
        circle.setHeight(circle.height() + step);
        qDebug() << circle;
    }

    return item;
}

QGraphicsItem *ellipse_reflected(QPointF &center, QPointF &cur, QColor color)
{
    QGraphicsItemGroup *item = new QGraphicsItemGroup();
    item->addToGroup(pixel(QPointF(center.x() + cur.x(), center.y() + cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() + cur.x(), center.y() - cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.x(), center.y() + cur.y()), color));
    item->addToGroup(pixel(QPointF(center.x() - cur.x(), center.y() - cur.y()), color));

    return item;
}

QGraphicsItem *ellipse_biblio(QRectF &circle, QColor color)
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(circle);
    item->setPen(QPen(color));

    return item;
}

QGraphicsItem *ellipse_canonical(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QRectF circle2 = QRectF(circle.x() + circle.width() / 2, circle.y() + circle.height() / 2, circle.width() / 2, circle.height() / 2);
    circle = circle2;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double aSqr = circle.width() * circle.width();
    double bSqr = circle.height() * circle.height();

    int limit = round(aSqr / sqrt(aSqr + bSqr));
    for (int x = 0; x <= limit; x++)
    {
        int y = round(sqrt(1 - x * x / aSqr) * circle.height());
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));
    }

    limit = round(bSqr / sqrt(aSqr + bSqr));
    for (int y = limit; y > -1; y--)
    {
        int x = round(sqrt(1 - y * y / bSqr) * circle.width());
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));
    }

    return item;
}

QGraphicsItem *ellipse_param(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QRectF circle2 = QRectF(circle.x() + circle.width() / 2, circle.y() + circle.height() / 2, circle.width() / 2, circle.height() / 2);
    circle = circle2;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double step = 1 / fmax(circle.width(), circle.height());
    for (double t = 0; t <= M_PI / 2; t += step)
    {
        int x = round(circle.width() * cos(t));
        int y = round(circle.height() * sin(t));
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));
    }

    return item;
}

QGraphicsItem *ellipse_bresenham(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QRectF circle2 = QRectF(circle.x() + circle.width() / 2, circle.y() + circle.height() / 2, circle.width() / 2, circle.height() / 2);
    circle = circle2;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    int aSqr = circle.width() * circle.width();
    int bSqr = circle.height() * circle.height();

    int x = 0;
    int y = circle.height();
    int d = bSqr - aSqr * (2 * circle.height() + 1);
    while (y >= 0)
    {
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));

        if (d < 0)
        {
            int d1 = 2 * d + aSqr * (2 * y + 2);
            x++;
            if (d1 < 0)
            {
                d += bSqr * (2 * x + 1);
            }
            else
            {
                y--;
                d += bSqr * (2 * x + 1) + aSqr * (1 - 2 * y);
            }
        }
        else if (d > 0)
        {
            int d2 = 2 * d + bSqr * (2 - 2 * x);
            y--;
            if (d2 > 0)
            {
                d += aSqr * (1 - 2 * y);
            }
            else
            {
                x++;
                d += bSqr * (2 * x + 1) + aSqr * (1 - 2 * y);
            }
        }
        else
        {
            x++;
            y--;
            d += bSqr * (2 * x + 1) + aSqr * (1 - 2 * y);
        }
    }

    return item;
}

QGraphicsItem *ellipse_midpoint(QRectF &circle, QColor color)
{
    int radius = circle.height() / 2;
    if (radius == 0)
        return nullptr;

    QRectF circle2 = QRectF(circle.x() + circle.width() / 2, circle.y() + circle.height() / 2, circle.width() / 2, circle.height() / 2);
    circle = circle2;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    int aSqr = circle.width() * circle.width();
    int bSqr = circle.height() * circle.height();

    int x = 0;
    int y = circle.height();

    int limit = round(circle.width() / sqrt(1 + (double)bSqr / aSqr));

    double d = bSqr - aSqr * (circle.height() - 0.25);
    while (x <= limit)
    {
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));

        if (d > 0)
        {
            y--;
            d -= aSqr * y * 2;
        }

        x++;
        d += bSqr * (2 * x + 1);
    }

    x = circle.width();
    y = 0;
    d = aSqr - bSqr * (circle.width() - 0.25);
    limit = round(circle.height() / sqrt(1 + (double)aSqr / bSqr));

    while (y <= limit)
    {
        QPointF c = QPointF(circle.x(), circle.y());
        QPointF cur = QPointF(x, y);
        item->addToGroup(ellipse_reflected(c, cur, color));

        if (d > 0)
        {
            x--;
            d -= 2 * bSqr * x;
        }

        y++;
        d += aSqr * (2 * y + 1);
    }

    return item;
}
