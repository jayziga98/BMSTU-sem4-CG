#include "spectrum.h"

QGraphicsItem *spectrum(QPointF &c, qreal len, qreal angle, line_params_t &params)
{
    QGraphicsItemGroup *item = new QGraphicsItemGroup();

    qreal rad = angle * PI / 180;

    for (qreal i = 0.0; i <= 2 * PI; i += rad)
    {
        QPointF end = QPointF(c.x() + cos(i) * len, c.y() - sin(i) * len);

        QLineF new_line = QLineF(c, end);

        item->addToGroup(line(new_line, params, nullptr));
    }

    return item;
}
