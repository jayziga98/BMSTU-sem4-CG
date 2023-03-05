#include "graphicsview.h"
#include "drawer.h"
#include "vector.h"

#include <QDrag>
#include <qstring.h>
#include <QDragEnterEvent>
#include <QMimeData>

graphicsView::graphicsView(QWidget *parent) : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);

    this->setScene(scene);
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->setStyleSheet("QGraphicsView {background-color: white}");
    this->setMouseTracking(true);

    this->linesGroup = nullptr;
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

void graphicsView::setupScene()
{
    auto rcontent = this->contentsRect();
    this->setSceneRect(0, 0, rcontent.width(), rcontent.height());
}

void graphicsView::mousePressEvent(QMouseEvent *event)
{
    this->linesGroup = this->removeItems(this->linesGroup);

    if(event->modifiers().testFlag(Qt::ShiftModifier)) {
        setDragMode(QGraphicsView::ScrollHandDrag);
        qDebug() << "setting dragging mode";
        QGraphicsView::mousePressEvent(event);
    }

    this->setupScene();
    drawer_t drawer;
    init(this->scene(), drawer, Qt::black, Qt::black);

    QPointF pos = mapToScene(event->pos());

    point_t p;
    init(p, pos.x(), pos.y());

    QGraphicsItem *item = drawer.scene->itemAt(pos, QTransform());
    if(event->buttons().testFlag(Qt::LeftButton) && !event->modifiers().testFlag(Qt::ControlModifier))
    {
        if (this->ToDraw() == POINT)
            draw_point(drawer, p);
        else if (!startDrawing && this->ToDraw() == CIRCLE)
        {
            circle_t c;
            init(c, pos.x(), pos.y(), POINT_RADIUS * 2);
            basePoint = pos;
            draw_circle(drawer, c);
            startDrawing = drawer.scene->itemAt(pos, QTransform());
        }
    }
    else
    {
        if (event->buttons().testFlag(Qt::RightButton))
            drawer.scene->removeItem(item);
        this->setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void graphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());

    QGraphicsItem *item = this->scene()->itemAt(pos, QTransform());
    if (item != nullptr)
    {
        if (startDrawing)
        {
            vector_t vrad;
            init(vrad, basePoint.x(), basePoint.y(), pos.x(), pos.y());
            double rad = length(vrad) + startDrawing->boundingRect().width() / 4;
            QGraphicsEllipseItem *castedItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(startDrawing);
            castedItem->setRect(basePoint.x() - rad, basePoint.y() - rad, rad * 2, rad * 2);
        }

        if (event->buttons().testFlag(Qt::LeftButton))
            this->setCursor(Qt::ClosedHandCursor);
        else
            item->setCursor(Qt::PointingHandCursor);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void graphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    this->linesGroup = this->removeItems(this->linesGroup);

    this->setCursor(Qt::ArrowCursor);
    startDrawing = 0;

    QPointF pos = mapToScene(event->pos());

    QGraphicsItem *item = this->scene()->itemAt(pos, QTransform());
    if (item != nullptr)
    {
        item->setFlag(QGraphicsEllipseItem::ItemIsMovable);
        QString toolTip = item->toolTip();
        double r = 0;
        if (toolTip.contains('r'))
        {
            r = toolTip.right(toolTip.indexOf(')') - toolTip.indexOf('r') - 3).toDouble();
            item->setToolTip(QString("(x: %1, ").arg(pos.x(), 0, 'g', 3) + QString("y: %1, ").arg(pos.y(), 0, 'g', 3) + QString("r: %1)").arg(r, 0, 'g', 3));
        }
        else
            item->setToolTip(QString("(x: %1, ").arg(pos.x(), 0, 'g', 3) + QString("y: %1)").arg(pos.y(), 0, 'g', 3));
    }

    QGraphicsView::mouseReleaseEvent(event);
}

QGraphicsItemGroup *graphicsView::getlinesGroup()
{
    return this->linesGroup;
}

void graphicsView::createlinesGroup()
{
    this->linesGroup = new QGraphicsItemGroup();
    this->scene()->addItem(this->linesGroup);
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
