#include "staskpool.h"

STaskPool::STaskPool(QObject *parent) : QObject(parent)
{
    last_id = 0;
    redo_ = stack.createRedoAction(this, "Повторить");
    undo_ = stack.createUndoAction(this, "Отменить");

    undo_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
    redo_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z));
}

STask *STaskPool::allocateTask(QGraphicsScene *scene, QGraphicsItem *item)
{
    STask *task = new STask(scene, item, last_id++);
    return task;
}

QAction *STaskPool::getUndoCommand()
{
    return undo_;
}

QAction *STaskPool::getRedoCommand()
{
    return redo_;
}

void STaskPool::sendTask(STask *task)
{
    stack.push(task);
}

void STaskPool::allocAndSend(QGraphicsScene *scene, QGraphicsItem *item)
{
    STask *task = allocateTask(scene, item);
    sendTask(task);
}
