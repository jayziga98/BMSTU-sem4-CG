#ifndef LINE_H
#define LINE_H

#include "QtGui/qcolor.h"
#include "QtWidgets/qgraphicsscene.h"
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QtCore>

enum algoType {
    CDA,
    BRESENHAM_FLOAT,
    BRESENHAM_INT,
    BRESENHAM_NO_GRAD,
    WU,
    BIBLIO
};

typedef struct {
    algoType type;
    QColor color;
} line_params_t;

QGraphicsItem *line(QLineF &line, line_params_t &params);
QGraphicsItem *line_cda(QLineF &line, QColor color);
QGraphicsItem *line_bresenham_int(QLineF &line, QColor color);
QGraphicsItem *line_bresenham_float(QLineF &line, QColor color);
QGraphicsItem *line_bresenham_no_grad(QLineF &line, QColor color);
QGraphicsItem *line_wu(QLineF &line, QColor color);
QGraphicsItem *line_biblio(QLineF &line, QColor color);

#endif // LINE_H
