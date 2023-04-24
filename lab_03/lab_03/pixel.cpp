#include "pixel.h"
#include <qpen.h>

QGraphicsItem *pixel(QPointF p, QColor color)
{
    QGraphicsRectItem *pixel = new QGraphicsRectItem(p.x() - 0.5, p.y() - 0.5, 1, 1);
    pixel->setPen(QPen(Qt::transparent));
    pixel->setBrush(QBrush(color));

    return pixel;
}
