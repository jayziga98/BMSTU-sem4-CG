#include "staskpool.h"
#include "QtWidgets/qlistwidget.h"

STaskPool::STaskPool(QObject *parent) : QObject(parent)
{
    last_id = 0;
    redo_ = stack.createRedoAction(this, "Повторить");
    undo_ = stack.createUndoAction(this, "Отменить");

    undo_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
    redo_->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z));
}

void STaskPool::setWidget(QListWidget *w)
{
    this->widget = w;

    this->widget->setStyleSheet("QListWidget::item[separator=\"true\"] { background: rgb(87, 87, 87); border: 1px solid black; border-radius: 2px;}");
    this->widget->setProperty("separator", true);
    this->widget->setSpacing(1);
}

STask *STaskPool::allocateTask(actionType type, figure_item_t *item, std::vector <double> params)
{
    STask *task = new STask(this->widget, type, item, params, last_id++);
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

void STaskPool::allocAndSend(actionType type, figure_item_t *item, std::vector <double> params)
{
    STask *task = allocateTask(type, item, params);
    sendTask(task);
}
