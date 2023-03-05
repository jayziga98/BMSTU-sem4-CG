#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include "point.h"
#include "circle.h"

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

#endif // DRAWER_H
