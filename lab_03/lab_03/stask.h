#ifndef STASK_H
#define STASK_H

#include "QtWidgets/qgraphicsitem.h"
#include <QUndoCommand>

class STask : public QUndoCommand
{
    int task_id;

    QGraphicsItem *item = nullptr;
    QGraphicsScene *scene = nullptr;

public:
    STask(QGraphicsScene *scene, QGraphicsItem *item, int id);
    void redo();
    void undo();
};

#endif // STASK_H
