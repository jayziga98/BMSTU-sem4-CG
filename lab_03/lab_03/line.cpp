#include "line.h"
#include <QGraphicsItemGroup>
#include <QDebug>

template<class T>
static int sign(T num)
{
    if (num > 0)
        return 1;
    if (num < 0)
        return -1;
    return 0;
}

QGraphicsItem *line(QLineF &line, line_params_t &params, int *step)
{
    if (step)
        *step = 0;

    QGraphicsItem *item = nullptr;

    switch (params.type) {
    case CDA:
        item = line_cda(line, params.color, step);
        break;
    case BRESENHAM_FLOAT:
        item = line_bresenham_float(line, params.color, step);
        break;
    case BRESENHAM_INT:
        item = line_bresenham_int(line, params.color, step);
        break;
    case BRESENHAM_NO_GRAD:
        item = line_bresenham_no_grad(line, params.color, step);
        break;
    case WU:
        item = line_wu(line, params.color, step);
        break;
    case BIBLIO:
        item = line_biblio(line, params.color);
        break;
    }

    return item;
}

QGraphicsItem *line_biblio(QLineF &line, QColor color)
{
    QGraphicsLineItem *item = new QGraphicsLineItem(line);
    item->setPen(QPen(color));

    return item;
}

QGraphicsItem *pixel(QPointF p, QColor color)
{
    QGraphicsRectItem *pixel = new QGraphicsRectItem(p.x() - 0.5, p.y() - 0.5, 1, 1);
    pixel->setPen(QPen(Qt::transparent));
    pixel->setBrush(QBrush(color));

    return pixel;
}

QGraphicsItem *line_cda(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    QPointF start = line.p1();
    QPointF end = line.p2();

    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    int l = fmax(abs(iend.x() - istart.x()), abs(iend.y() - istart.y())) + 1;
    double inc_x = (end.x() - start.x()) / l;
    double inc_y = (end.y() - start.y()) / l;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    item->addToGroup(pixel(start, color));
    for (int i = 0; i < l; i++)
    {
        auto old = start;

        start.setX(start.x() + inc_x);
        start.setY(start.y() + inc_y);

        if (step && round(old.x()) != round(start.x()) && round(old.y()) != round(start.y()))
            (*step)++;

        item->addToGroup(pixel(start, color));
    }

    return item;
}

QGraphicsItem *line_bresenham_int(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    QPointF start = line.p1();
    QPointF end = line.p2();

    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    int dx = iend.x() - istart.x();
    int dy = iend.y() - istart.y();

    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;

    dx = abs(dx);
    dy = abs(dy);

    bool swapped = false;
    if (dy > dx)
    {
        swapped = true;
        std::swap(dx, dy);
    }

    int e = 2 * dy - dx;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    item->addToGroup(pixel(istart, color));
    for (int i = 0; i <= dx; i++)
    {
        auto old = istart;

        if (e >= 0)
        {
            istart.setX(istart.x() + sx);
            istart.setY(istart.y() + sy);

            e += 2 * dy - 2 * dx;
        }
        else {
            if (!swapped)
                istart.setX(istart.x() + sx);
            else
                istart.setY(istart.y() + sy);

            e += 2 * dy;
        }

        if (step && round(old.x()) != round(istart.x()) && round(old.y()) != round(istart.y()))
            (*step)++;

        item->addToGroup(pixel(istart, color));
    }

    return item;
}
QGraphicsItem *line_bresenham_float(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    int x = line.x1();
    int y = line.y1();

    int dx = abs(line.dx());
    int dy = abs(line.dy());

    auto sx = sign(line.dx());
    auto sy = sign(line.dy());

    bool swapped = false;
    if (dy > dx)
    {
        std::swap(dx, dy);
        swapped = true;
    }

    qreal m = (qreal)dy / dx;
    auto e = m - 0.5;
    int l = dx + 1;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    auto prev_x = x;
    auto prev_y = y;
    for (int i = 0; i < l; i++)
    {
        if (i > 0 && step && x != prev_x && y != prev_y)
            (*step)++;

        item->addToGroup(pixel(QPoint(x, y), color));
        prev_x = x;
        prev_y = y;

        if (e >= 0)
        {
            if (swapped)
            {
                x += sx;
            }
            else
            {
                y += sy;
            }
            --e;
        }

        if (!swapped)
        {
            x += sx;
        }
        else
        {
            y += sy;
        }

        e += m;
    }

    return item;
}

QGraphicsItem *line_bresenham_no_grad(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    int imax = 255;
    int cur_x = line.x1();
    int cur_y = line.y1();

    int dx = line.dx();
    int dy = line.dy();

    auto sx = sign(dx);
    auto sy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    bool change = false;
    if (dy > dx)
    {
        change = true;
        std::swap(dx, dy);
    }

    qreal m = (qreal)dy / dx * imax;
    qreal e = imax / 2.0;
    qreal w = imax - m;
    auto l = dx + 1;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    auto prev_x = cur_x;
    auto prev_y = cur_y;
    for (int i = 0; i < l; i++)
    {
        if (step && (prev_x != cur_x && prev_y != cur_y))
        {
            ++(*step);
        }

        color.setAlpha(round(e));
        item->addToGroup(pixel(QPoint(cur_x, cur_y), color));
        prev_x = cur_x;
        prev_y = cur_y;
        if (e <= w)
        {
            if (change)
            {
                cur_y += sy;
            }
            else
            {
                cur_x += sx;
            }
            e += m;
        }
        else
        {
            cur_x += sx;
            cur_y += sy;
            e -= w;
        }
    }
    return item;
}

static int iPartOfNumber(qreal x)
{
    return floor(x);
}

static qreal fPartOfNumber(qreal x)
{
    return x - iPartOfNumber(x);
}

static qreal rfPartOfNumber(qreal x)
{
    return 1 - fPartOfNumber(x);
}

QGraphicsItem *line_wu(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    int imax = 255;
    int steep = std::abs(line.dy()) > std::abs(line.dx());
    int x0 = line.x1(), x1 = line.x2();
    int y0 = line.y1(), y1 = line.y2();
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    qreal dx = x1 - x0;
    qreal dy = y1 - y0;
    qreal gradient = dx ? dy / dx : 1;

    int xpxl1 = x0;
    int xpxl2 = x1;
    qreal intersectY = y0;
    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    if (steep)
    {
        int x;
        for (x = xpxl1; x <= xpxl2; ++x)
        {
            color.setAlpha(imax * rfPartOfNumber(intersectY));
            item->addToGroup(pixel(QPoint(intersectY, x), color));

            color.setAlpha(imax * fPartOfNumber(intersectY));
            item->addToGroup(pixel(QPoint(intersectY + 1, x), color));

            if (step && round(intersectY) != round(intersectY + gradient))
                (*step)++;
            intersectY += gradient;
        }
    }
    else
    {
        int x;
        for (x = xpxl1; x <= xpxl2; ++x)
        {
            color.setAlpha(imax * rfPartOfNumber(intersectY));
            item->addToGroup(pixel(QPoint(x, intersectY), color));

            color.setAlpha(imax * fPartOfNumber(intersectY));
            item->addToGroup(pixel(QPoint(x, intersectY + 1), color));

            if (step && round(intersectY) != round(intersectY + gradient))
                (*step)++;
            intersectY += gradient;
        }
    }

    return item;
}
