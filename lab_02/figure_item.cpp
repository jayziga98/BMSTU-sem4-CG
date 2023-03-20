#include "figure_item.h"

figure_item_t *figure_item_create()
{
    return new figure_item_t();
}

void figure_item_add_line(figure_item_t *item, point_t &p1, point_t &p2)
{
    item->addToGroup(new QGraphicsLineItem(p1.x, p1.y, p2.x, p2.y));
}

void figure_item_add_circle(figure_item_t *item, point_t &p1, point_t &p2)
{
    double r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    item->addToGroup(new QGraphicsEllipseItem(p1.x - r, p1.y - r, r, r));
}

void figure_item_init(figure_item_t *item, figure_t &figure)
{
    for (auto link: figure.links)
    {
        if (link.type != LINE)
            qDebug() << link.type << LINE;
        switch (link.type) {
        case LINE:
            figure_item_add_line(item, figure.points[link.first], figure.points[link.second]);
            break;
        case CIRCLE:
            qDebug() << link.type << CIRCLE << LINE;
            figure_item_add_circle(item, figure.points[link.first], figure.points[link.second]);
            break;
        default:
            figure_item_add_line(item, figure.points[link.first], figure.points[link.second]);
        }
    }
}

void figure_item_move(figure_item_t *item, double dx, double dy)
{
    for (auto child: item->childItems())
    {
        figure_item_move_line(child, dx, dy);
        figure_item_move_circle(child, dx, dy);
    }
}

void figure_item_rotate(figure_item_t *item, double deg, bool base, double basex, double basey)
{
    QRectF childRect = item->childrenBoundingRect();

    if (!base)
    {
        basex = childRect.x() + childRect.width() / 2;
        basey = childRect.y() + childRect.height() / 2;
    }

    for (auto child: item->childItems())
    {
        figure_item_rotate_line(child, deg, basex, basey);
        figure_item_rotate_circle(child, deg, basex, basey);
    }
}

void figure_item_scale(figure_item_t *item, double kx, double ky, bool base, double basex, double basey)
{
    QRectF childRect = item->childrenBoundingRect();

    if (!base)
    {
        basex = childRect.x() + childRect.width() / 2;
        basey = childRect.y() + childRect.height() / 2;
    }

    for (auto child: item->childItems())
    {
        figure_item_scale_line(child, kx, ky, basex, basey);
        figure_item_scale_circle(child, kx, ky, basex, basey);
    }
}

void figure_item_move_line(QGraphicsItem *line_item, double dx, double dy)
{
    QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(line_item);
    if (line == nullptr)
        return;

    QPointF qp1 = line->line().p1();
    QPointF qp2 = line->line().p2();

    point_t p1 = point_init(qp1.x(), qp1.y(), 0);
    point_t p2 = point_init(qp2.x(), qp2.y(), 0);

    point_move(p1, dx, dy);
    point_move(p2, dx, dy);

    qp1 = QPointF(p1.x, p1.y);
    qp2 = QPointF(p2.x, p2.y);

    line->setLine(QLineF(qp1, qp2));
}

void figure_item_rotate_line(QGraphicsItem *line_item, double deg, double basex, double basey)
{
    QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(line_item);
    if (line == nullptr)
        return;

    QPointF qp1 = line->line().p1();
    QPointF qp2 = line->line().p2();

    point_t p1 = point_init(qp1.x(), qp1.y(), 0);
    point_t p2 = point_init(qp2.x(), qp2.y(), 0);

    point_move(p1, -basex, -basey);
    point_move(p2, -basex, -basey);

    point_rotate(p1, deg);
    point_rotate(p2, deg);

    point_move(p1, basex, basey);
    point_move(p2, basex, basey);

    qp1 = QPointF(p1.x, p1.y);
    qp2 = QPointF(p2.x, p2.y);

    line->setLine(QLineF(qp1, qp2));
}

void figure_item_scale_line(QGraphicsItem *line_item, double kx, double ky, double basex, double basey)
{
    QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem *>(line_item);
    if (line == nullptr)
        return;

    QPointF qp1 = line->line().p1();
    QPointF qp2 = line->line().p2();

    point_t p1 = point_init(qp1.x(), qp1.y(), 0);
    point_t p2 = point_init(qp2.x(), qp2.y(), 0);

    point_move(p1, -basex, -basey);
    point_move(p2, -basex, -basey);

    point_scale(p1, kx, ky);
    point_scale(p2, kx, ky);

    point_move(p1, basex, basey);
    point_move(p2, basex, basey);

    qp1 = QPointF(p1.x, p1.y);
    qp2 = QPointF(p2.x, p2.y);

    line->setLine(QLineF(qp1, qp2));
}

void figure_item_move_circle(QGraphicsItem *circle_item, double dx, double dy)
{
    QGraphicsEllipseItem *circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(circle_item);
    if (circle == nullptr)
        return;

    QPointF qcenter = circle->rect().center();
    double r = circle->rect().width() / 2;

    point_t center = point_init(qcenter.x(), qcenter.y(), 0);

    point_move(center, dx, dy);

    qcenter = QPointF(center.x, center.y);

    circle->setRect(QRectF(qcenter.x() - r, qcenter.y() - r, r, r));
}

void figure_item_rotate_circle(QGraphicsItem *circle_item, double deg, double basex, double basey)
{
    QGraphicsEllipseItem *circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(circle_item);
    if (circle == nullptr)
        return;

    QPointF qcenter = circle->rect().center();
    double r = circle->rect().width() / 2;

    point_t center = point_init(qcenter.x(), qcenter.y(), 0);

    point_move(center, -basex, -basey);

    point_rotate(center, deg);

    point_move(center, basex, basey);

    qcenter = QPointF(center.x, center.y);

    circle->setRect(QRectF(qcenter.x() - r, qcenter.y() - r, r, r));
}

void figure_item_scale_circle(QGraphicsItem *circle_item, double kx, double ky, double basex, double basey)
{
    QGraphicsEllipseItem *circle = qgraphicsitem_cast<QGraphicsEllipseItem *>(circle_item);
    if (circle == nullptr)
        return;

    QPointF qcenter = circle->rect().center();
    double r = circle->rect().width() / 2;

    point_t center = point_init(qcenter.x(), qcenter.y(), 0);

    point_move(center, -basex, -basey);

    point_scale(center, kx, ky);

    point_move(center, basex, basey);

    qcenter = QPointF(center.x, center.y);

    circle->setRect(QRectF(qcenter.x() - r, qcenter.y() - r, r, r));
}

