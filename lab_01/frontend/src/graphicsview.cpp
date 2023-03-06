#include "graphicsview.h"
#include "drawer.h"

#include <QDrag>
#include <qstring.h>
#include <QDragEnterEvent>
#include <QGestureEvent>
#include <QMimeData>

graphicsView::graphicsView(QWidget *parent) : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);

    this->setScene(scene);
    qDebug() << this->scene();
    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setStyleSheet("QGraphicsView {background-color: white}");
    this->setMouseTracking(true);

    this->linesGroup = nullptr;
    this->todraw = POINT;

    this->scale(1, -1);

    this->setupScene();
    drawer_t drawer;
    init(this->scene(), drawer, Qt::black, Qt::black);

    viewport()->grabGesture(Qt::PinchGesture);
}

void graphicsView::setupScene()
{
    auto rcontent = this->contentsRect();
    this->setSceneRect(0, rcontent.height(), rcontent.width(), -rcontent.height());
}

void graphicsView::clearScene()
{
    this->setupScene();
    drawer_t drawer;
    init(this->scene(), drawer, Qt::black, Qt::black);

    QList <QGraphicsItem *> items = drawer.scene->items();
    for (auto item: items)
        remove_item(drawer, item);

    this->drawAxis();
    //this->fitInView(this->sceneRect(), Qt::KeepAspectRatio);
}

void graphicsView::clearSelected()
{
    this->setupScene();
    drawer_t drawer;
    init(this->scene(), drawer, Qt::black, Qt::black);

    QList <QGraphicsItem *> items = drawer.scene->selectedItems();
    for (auto item: items)
        remove_item(drawer, item);

    //this->fitInView(this->sceneRect(), Qt::KeepAspectRatio);
}

void graphicsView::fitAll()
{
    this->setupScene();
    QRectF sceneBounds = this->scene()->itemsBoundingRect();

    if (sceneBounds.contains(this->sceneRect()))
    {
        this->fitInView(sceneBounds, Qt::KeepAspectRatio);
        this->centerOn(0, 0);
    }
}

void graphicsView::drawAxis()
{
    QRectF sceneBounds = this->sceneRect();
    if (this->axisX)
        this->scene()->removeItem(this->axisX);
    if (this->axisY)
        this->scene()->removeItem(this->axisY);
    this->axisX = this->scene()->addLine(QLineF(-sceneBounds.width(), 0, sceneBounds.width(), 0), QPen(Qt::gray, 4, Qt::DashLine));
    this->axisY = this->scene()->addLine(QLineF(0, sceneBounds.height(), 0, -sceneBounds.height()), QPen(Qt::gray, 4, Qt::DashLine));
    this->fitAll();
}

void graphicsView::mousePressEvent(QMouseEvent *event)
{
    this->linesGroup = this->removeItems(this->linesGroup);

    this->setCursor(Qt::ArrowCursor);

    this->setupScene();
    drawer_t drawer;
    init(this->scene(), drawer, Qt::black, Qt::black);

    QPointF pos = mapToScene(event->pos());

    QGraphicsItem *item = drawer.scene->itemAt(pos, QTransform());
    if(event->buttons().testFlag(Qt::LeftButton) && !event->modifiers().testFlag(Qt::ControlModifier))
    {
        basePoint = pos;
        startDrawing = draw(drawer, pos, this->ToDraw());
        event->buttons().setFlag(Qt::LeftButton);
    }
    else if (event->buttons().testFlag(Qt::RightButton))
        remove_item(drawer, item);

    QGraphicsView::mousePressEvent(event);
}

void graphicsView::mouseMoveEvent(QMouseEvent *event)
{
    this->setupScene();
    QPointF pos = mapToScene(event->pos());

    drawAxis();

    QGraphicsItem *item = this->scene()->itemAt(pos, QTransform());

    resize_circle(startDrawing, basePoint, pos);

    if (item != nullptr)
    {
        if (event->buttons().testFlag(Qt::LeftButton))
            this->setCursor(Qt::ClosedHandCursor);
        else
            item->setCursor(Qt::PointingHandCursor);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void graphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    this->fitAll();

    this->linesGroup = this->removeItems(this->linesGroup);

    this->setCursor(Qt::ArrowCursor);

    startDrawing = 0;

    QPointF pos = mapToScene(event->pos());

    QGraphicsItem *item = this->scene()->itemAt(pos, QTransform());
    update_tooltip(item, pos);

    QGraphicsView::mouseReleaseEvent(event);
}

bool graphicsView::viewportEvent(QEvent *event)
{
    this->setupScene();
    if (event->type() == QEvent::Gesture)
    {
        gestureEvent(static_cast<QGestureEvent*>(event));
    }
    else if (event->type() == QEvent::TouchBegin)
    {
      return false;
    }

    return QGraphicsView::viewportEvent(event);
}

void graphicsView::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pinchGesture = event->gesture(Qt::PinchGesture))
    {
        QPinchGesture *pinch = static_cast<QPinchGesture *>(pinchGesture);

        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        scale(pinch->scaleFactor(), pinch->scaleFactor());
        setTransformationAnchor(anchor);
    }
}

void graphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
        this->clearSelected();
}

void graphicsView::createlinesGroup()
{
    this->linesGroup = new QGraphicsItemGroup();
    this->scene()->addItem(this->linesGroup);
}

QGraphicsItemGroup *graphicsView::removeItems(QGraphicsItemGroup *group)
{
    if (group != nullptr)
    {
        qDeleteAll(group->childItems());

        this->scene()->destroyItemGroup(group);
    }
    return nullptr;
}

QGraphicsItemGroup *graphicsView::getlinesGroup()
{
    return this->linesGroup;
}

void graphicsView::addlinesGroup(QGraphicsItem *item)
{
    this->linesGroup->addToGroup(item);
}

void graphicsView::setToDraw(DrawerItem newToDraw){
    this->todraw = newToDraw;
}

DrawerItem graphicsView::ToDraw()
{
    return this->todraw;
}
