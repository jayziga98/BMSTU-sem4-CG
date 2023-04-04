#include "line.h"
#include <QGraphicsItemGroup>
#include <QDebug>

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

    double m = dx ? dy / dx : 1;

    double e = m - 0.5;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    item->addToGroup(pixel(start, color));
    for (int i = 0; i <= int(dx); i++)
    {
        auto old = istart;

        if (e >= 0)
        {
            istart.setX(istart.x() + sx);
            istart.setY(istart.y() + sy);

            e -= 1;
        }
        else {
            if (!swapped)
                istart.setX(istart.x() + sx);
            else
                istart.setY(istart.y() + sy);

            e += m;
        }

        if (step && round(old.x()) != round(istart.x()) && round(old.y()) != round(istart.y()))
            (*step)++;


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
        auto old = istart;

        if (e >= w)
        {
            istart.setY(istart.y() + sy);
            istart.setX(istart.x() + sx);

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

        if (step && round(old.x()) != round(istart.x()) && round(old.y()) != round(istart.y()))
            (*step)++;

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

        QPointF istart = QPointF(line.p1().x(), line.p1().y());
        QPointF iend = QPointF(line.p2().x(), line.p2().y());

//    QPoint istart = QPoint(round(line.p1().x()), round(line.p1().y()));
//    QPoint iend = QPoint(round(line.p2().x()), round(line.p2().y()));

    double dx = iend.x() - istart.x();
    double dy = iend.y() - istart.y();

    dx = abs(dx);
    dy = abs(dy);

    bool swapped = false;
    if (dy > dx)
    {
        swapped = true;
        istart = QPointF(line.p1().y(), line.p1().x());
        iend = QPointF(line.p2().y(), line.p2().x());
//        istart = QPoint(round(line.p1().y()), round(line.p1().x()));
//        iend = QPoint(round(line.p2().y()), round(line.p2().x()));
    }

    if (istart.x() > iend.x())
        std::swap(istart, iend);

    dx = iend.x() - istart.x();
    dy = iend.y() - istart.y();

    double m = dx ? dy / dx : 1;

    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    double y = istart.y() + m;
    double x = istart.x();
    for (int i = 0; i <= dx; i++)
    {   
        int s = y > 0 ? 1 : -1;

        auto r1 = abs(y) - (int)abs(y);
        auto r2 = 1 - r1;

//        if (s < 0)
//            std::swap(r1, r2);

        QPoint cur1 = QPoint(x, y);
        if (swapped)
        {
            cur1.setX(y);
            cur1.setY(x);
        }

        color.setAlpha(255 * r2);
        item->addToGroup(pixel(cur1, color));

        QPoint cur2 = QPoint(x, y + s);
        if (swapped)
        {
            cur2.setX(y + s);
            cur2.setY(x);
        }

        color.setAlpha(255 * r1);
        item->addToGroup(pixel(cur2, color));

        if (step && round(y) != round(y + m))
            (*step)++;

        y += m;
        x += 1;
    }

    return item;
}
