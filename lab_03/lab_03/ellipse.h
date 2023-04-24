#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "QtGui/qcolor.h"
#include "QtWidgets/qgraphicsitem.h"
#include "algoTypes.h"

QGraphicsItem *ellipse_reflected(QPointF &center, QPointF &cur, QColor color);
QGraphicsItem *ellipse(QRectF &circle, params_t &params, int *step);
QGraphicsItem *ellipse_spectrum(QRectF &circle, params_t &params, double step, int amount);
QGraphicsItem *ellipse_biblio(QRectF &circle, QColor color);
QGraphicsItem *ellipse_canonical(QRectF &circle, QColor color);
QGraphicsItem *ellipse_param(QRectF &circle, QColor color);
QGraphicsItem *ellipse_bresenham(QRectF &circle, QColor color);
QGraphicsItem *ellipse_midpoint(QRectF &circle, QColor color);

#endif // ELLIPSE_H
