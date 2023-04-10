#ifndef CIRCLE_H
#define CIRCLE_H

#include "QtGui/qcolor.h"
#include "QtWidgets/qgraphicsscene.h"
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QtCore>

enum algoTypeCircle {
    CANONICAL,
    PARAMETRIC,
    BRESENHAM,
    MID_POINT,
    BIBLIO
};

typedef struct {
    algoTypeCircle type;
    QColor color;
} circle_params_t;

QGraphicsItem *circle(QRectF &circle, circle_params_t &params, int *step);
QGraphicsItem *circle_biblio(QRectF &circle, QColor color);

#endif // CIRCLE_H
