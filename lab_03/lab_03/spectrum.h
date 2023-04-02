#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QtCore>
#include "line.h"
#include <math.h>

const double PI = acos(-1);

QGraphicsItem *spectrum(QPointF &c, qreal len, qreal angle, line_params_t &params);

#endif // SPECTRUM_H
