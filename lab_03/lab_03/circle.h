#ifndef CIRCLE_H
#define CIRCLE_H

#include "QtGui/qcolor.h"
#include "QtWidgets/qgraphicsscene.h"
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QtCore>
#include "algoTypes.h"

QGraphicsItem *reflected(QPointF &center, QPointF &cur, QColor color);
QGraphicsItem *circle(QRectF &circle, params_t &params, int *step);
QGraphicsItem *circle_spectrum(QRectF &c, params_t &params, double step, int amount);
QGraphicsItem *circle_biblio(QRectF &circle, QColor color);
QGraphicsItem *circle_canonical(QRectF &circle, QColor color);
QGraphicsItem *circle_param(QRectF &circle, QColor color);
QGraphicsItem *circle_bresenham(QRectF &circle, QColor color);
QGraphicsItem *circle_midpoint(QRectF &circle, QColor color);

#endif // CIRCLE_H
