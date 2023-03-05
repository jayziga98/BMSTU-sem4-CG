#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "drawer.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <qgraphicsscene.h>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>

class graphicsView : public QGraphicsView
{
    Q_OBJECT
    DrawerItem todraw = POINT;

public:
    explicit graphicsView(QWidget *parent = nullptr);
    void setupScene();
    void setToDraw(DrawerItem);
    DrawerItem ToDraw();

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QPointF basePoint;
    QGraphicsItem *startDrawing;
};

#endif // GRAPHICSVIEW_H
