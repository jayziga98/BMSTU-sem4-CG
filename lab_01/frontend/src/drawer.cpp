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
    QGraphicsLineItem *item = scene.scene->addLine(QLineF(v1.x, v1.y, v2.x, v2.y), QPen(Qt::red, 7));
    item->setToolTip(QString("(x1: %1, ").arg(v1.x, 0, 'g', 3) + QString("y1: %1) -> ").arg(v1.y, 0, 'g', 3) + \
                     QString("(x2: %1, ").arg(v2.x, 0, 'g', 3) + QString("y2: %1)").arg(v2.y, 0, 'g', 3) + tooltipAdditional);
    item->setFlag(QGraphicsEllipseItem::ItemIsFocusable);
    item->setFlag(QGraphicsEllipseItem::ItemIsSelectable);

    return item;
}

QGraphicsItem *draw(const drawer_t &drawer, QPointF base, DrawerItem dItem)
{
    QGraphicsItem *startDrawing = nullptr;

    switch (dItem) {
        case POINT:
            point_t p;
            init(p, base.x(), base.y());
            draw_point(drawer, p);
        break;
        case CIRCLE:
            circle_t c;
            init(c, base.x(), base.y(), POINT_RADIUS * 2);
            draw_circle(drawer, c);
            startDrawing = drawer.scene->itemAt(base, QTransform());
        break;
    }

    return startDrawing;
}

void remove_item(drawer_t &drawer, QGraphicsItem *item)
{
    drawer.scene->removeItem(item);
}

QGraphicsItem *update_tooltip(QGraphicsItem *item, QPointF base)
{
    if (item != nullptr)
    {
        item->setFlag(QGraphicsEllipseItem::ItemIsMovable);
        QString toolTip = item->toolTip();
        double r = 0;
        if (toolTip.contains('r'))
        {
            r = toolTip.right(toolTip.indexOf(')') - toolTip.indexOf('r') - 3).toDouble();
            if (r == 0)
                r = item->boundingRect().width() / 2;
            item->setToolTip(QString("(x: %1, ").arg(base.x(), 0, 'g', 3) + QString("y: %1, ").arg(base.y(), 0, 'g', 3) + QString("r: %1)").arg(r, 0, 'g', 3));
        }
        else
            item->setToolTip(QString("(x: %1, ").arg(base.x(), 0, 'g', 3) + QString("y: %1)").arg(base.y(), 0, 'g', 3));
    }

    return item;
}

QGraphicsItem *resize_circle(QGraphicsItem *item, QPointF base, QPointF curPos)
{
    if (item == nullptr)
        return item;

    vector_t vrad;
    init(vrad, base.x(), base.y(), curPos.x(), curPos.y());
    double rad = length(vrad) + item->boundingRect().width() / 4;
    QGraphicsEllipseItem *castedItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(item);
    castedItem->setRect(base.x() - rad, base.y() - rad, rad * 2, rad * 2);

    return castedItem;
}
