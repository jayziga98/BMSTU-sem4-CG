#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "drawer.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <qgraphicsscene.h>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QGesture>

class graphicsView : public QGraphicsView
{
    Q_OBJECT
    DrawerItem todraw = POINT;
    QGraphicsItem *axisX = nullptr;
    QGraphicsItem *axisY = nullptr;

public:
    explicit graphicsView(QWidget *parent = nullptr);
    void setupScene();
    void setToDraw(DrawerItem);
    DrawerItem ToDraw();
    QGraphicsItemGroup *getlinesGroup();
    void createlinesGroup();
    void addlinesGroup(QGraphicsItem *item);
    void clearScene();
    void clearSelected();
    void fitAll();
    void drawAxis();

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void gestureEvent(QGestureEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool viewportEvent(QEvent *event);
private:
    QPointF basePoint;
    QGraphicsItem *startDrawing;
    QGraphicsItemGroup *linesGroup;
    QGraphicsItemGroup *removeItems(QGraphicsItemGroup *group);
};

#endif // GRAPHICSVIEW_H
