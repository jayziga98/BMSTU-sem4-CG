#include "stask.h"
#include <QGraphicsScene>

STask::STask(QGraphicsScene *scene, QGraphicsItem *item, int id)
{
    this->task_id = id;
    this->item = item;
    this->scene = scene;
}

void STask::redo()
{
    scene->addItem(item);
}

void STask::undo()
{
    scene->removeItem(item);
}
