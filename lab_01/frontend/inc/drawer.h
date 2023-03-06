#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include "point.h"
#include "circle.h"
#include "vector.h"

#define POINT_RADIUS 8

typedef struct {
    QGraphicsScene *scene;
    QPen pen;
    QBrush brush;
    double width;
    double height;
} drawer_t;

enum DrawerItem {
    POINT,
    CIRCLE
};

void init(QGraphicsScene *scene, drawer_t &drawer, Qt::GlobalColor outline, Qt::GlobalColor fill);
void setupItem(QGraphicsEllipseItem *item);
void draw_point(const drawer_t &scene, const point_t &p);
void draw_circle(const drawer_t &scene, const circle_t &c);
QGraphicsLineItem *draw_line(const drawer_t &scene, const vector_t &v1, const vector_t &v2, QString tooltipAdditional);
QGraphicsItem *draw(const drawer_t &drawer, QPointF base, DrawerItem dItem);
void remove_item(drawer_t &drawer, QGraphicsItem *item);
QGraphicsItem *update_tooltip(QGraphicsItem *item, QPointF base);
QGraphicsItem *resize_circle(QGraphicsItem *item, QPointF base, QPointF curPos);

#endif // DRAWER_H
