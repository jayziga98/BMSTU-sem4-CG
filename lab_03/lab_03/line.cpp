#include "line.h"
#include <QGraphicsItemGroup>

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
        start.setX(start.x() + inc_x);
        start.setY(start.y() + inc_y);

        if (step)
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
        if (e >= 0)
        {
            istart.setX(istart.x() + sx);
            istart.setY(istart.y() + sy);

            if (step)
                (*step)++;

            e += 2 * dy - 2 * dx;
        }
        else {
            if (!swapped)
                istart.setX(istart.x() + sx);
            else
                istart.setY(istart.y() + sy);

            e += 2 * dy;
        }

        item->addToGroup(pixel(istart, color));
    }

    return item;
}

QGraphicsItem *line_bresenham_float(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    QPointF start = line.p1();
    QPointF end = line.p2();

    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();

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

    double m = dy / dx;

    double e = m - 0.5;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    item->addToGroup(pixel(start, color));
    for (int i = 0; i <= dx; i++)
    {
        if (e >= 0)
        {
            istart.setX(istart.x() + sx);
            istart.setY(istart.y() + sy);

            if (step)
                (*step)++;

            e -= 1;
        }
        else {
            if (!swapped)
                istart.setX(istart.x() + sx);
            else
                istart.setY(istart.y() + sy);

            e += m;
        }


        item->addToGroup(pixel(start, color));
    }

    return item;
}

QGraphicsItem *line_bresenham_no_grad(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    QPointF start = line.p1();
    QPointF end = line.p2();

    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();

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

    int i = 255;

    double m = dy / dx * i;
    double w = i - m;
    double e = i / 2;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

//    color.setAlpha(m / 2);
//    item->addToGroup(pixel(start, color));
    for (int i = 0; i <= dx; i++)
    {
        if (e >= w)
        {
            istart.setY(istart.y() + sy);
            istart.setX(istart.x() + sx);

            if (step)
                (*step)++;

            e -= w;
        }
        else
        {
            if (!swapped)
                istart.setX(istart.x() + sx);
            else
                istart.setY(istart.y() + sy);

            e += m;
        }

        color.setAlpha(fmax(0, fmin(255, e)));
        item->addToGroup(pixel(istart, color));

        e += m;
    }

    return item;
}

QGraphicsItem *line_wu(QLineF &line, QColor color, int *step)
{
    if (step)
        *step = 0;

    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    double dx = iend.x() - istart.x();
    double dy = iend.y() - istart.y();

    dx = abs(dx);
    dy = abs(dy);

    bool swapped = false;
    if (dy > dx)
    {
        swapped = true;
        std::swap(dx, dy);
    }

    if (istart.x() > iend.x())
        std::swap(istart, iend);

    double m = 1;
    if (dx)
        m = dy / dx;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double y = istart.y() + m;
    for (int x = istart.x() + 1; x <= iend.x() - 1; x++)
    {
        QPoint cur = QPoint(x, y);
        if (swapped)
        {
            cur.setX(y);
            cur.setY(x);
        }

        istart.setX(cur.x());

        istart.setY(cur.y());
        color.setAlpha(255 - (y - (int)y) * 255);
        item->addToGroup(pixel(istart, color));

        istart.setY(cur.y() + 1);
        color.setAlpha((y - (int)y) * 255);
        item->addToGroup(pixel(istart, color));

        y += m;
    }

    return item;
}
