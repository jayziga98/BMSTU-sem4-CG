#include "drawlabel.h"
#include <QtGui/QtGui>

DrawLabel::DrawLabel(QWidget *widget) : QLabel(widget) {}

void DrawLabel::setPixmapPointer(QPixmap &rpixmap)
{
    pixmap = &rpixmap;
    this->setScaledContents(true);
}

void DrawLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.drawPixmap(QPoint(0, 0), *pixmap);
    painter.end();
}
