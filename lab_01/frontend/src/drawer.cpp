#include "drawer.h"
#include "QtWidgets/qgraphicsitem.h"
#include "vector.h"
#include <qstring.h>

void init(QGraphicsScene *scene, drawer_t &drawer, Qt::GlobalColor outline, Qt::GlobalColor fill)
{
    drawer.scene = scene;
    drawer.pen.setColor(outline);
    drawer.brush.setColor(fill);
    drawer.width = (double)scene->width();
    drawer.height = (double)scene->height();
}

void setupItem(QGraphicsEllipseItem *item)
{
    item->setFlag(QGraphicsEllipseItem::ItemIsSelectable);
    item->setFlag(QGraphicsEllipseItem::ItemSendsGeometryChanges);
    item->setFlag(QGraphicsEllipseItem::ItemIsFocusable);
    item->setAcceptDrops(true);
}

void draw_point(const drawer_t &scene, const point_t &p)
{
    QGraphicsEllipseItem *item = scene.scene->addEllipse(p.x - POINT_RADIUS / 2, p.y - POINT_RADIUS / 2, POINT_RADIUS, POINT_RADIUS, scene.pen, Qt::black);
    item->setToolTip(QString("(x: %1, ").arg(p.x, 0, 'g', 3) + QString("y: %1)").arg(p.y, 0, 'g', 3));
    item->setFlag(QGraphicsEllipseItem::ItemIsMovable);
    setupItem(item);
}

void draw_circle(const drawer_t &scene, const circle_t &c)
{
    QGraphicsEllipseItem *item = scene.scene->addEllipse(c.x - c.r, c.y - c.r, c.r * 2, c.r * 2, scene.pen);
    item->setToolTip(QString("(x: %1, ").arg(c.x, 0, 'g', 3) + QString("y: %1, ").arg(c.y, 0, 'g', 3) + QString("r: %1)").arg(c.r, 0, 'g', 3));
    setupItem(item);
}

QGraphicsLineItem *draw_line(const drawer_t &scene, const vector_t &v1, const vector_t &v2, QString tooltipAdditional)
{
    QGraphicsLineItem *item = scene.scene->addLine(QLineF(v1.x, v1.y, v2.x, v2.y), QPen(Qt::red, 4));
    item->setToolTip(QString("(x1: %1, ").arg(v1.x, 0, 'g', 3) + QString("y1: %1) -> ").arg(v1.y, 0, 'g', 3) + \
                     QString("(x2: %1, ").arg(v2.x, 0, 'g', 3) + QString("y2: %1)").arg(v2.y, 0, 'g', 3) + tooltipAdditional);
    item->setFlag(QGraphicsEllipseItem::ItemIsFocusable);
    item->setFlag(QGraphicsEllipseItem::ItemIsSelectable);

    return item;
}
