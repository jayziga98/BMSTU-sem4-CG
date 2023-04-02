#ifndef STASKPOOL_H
#define STASKPOOL_H

#include <QObject>
#include <QUndoStack>
#include <QAction>
#include "stask.h"

class STask;

class STaskPool : public QObject
{
    Q_OBJECT

    QUndoStack stack;

    int last_id = 0;

    QAction *undo_;
    QAction *redo_;
public:
    explicit STaskPool(QObject *parent = 0);
    QAction *getUndoCommand();
    QAction *getRedoCommand();

    STask *allocateTask(QGraphicsScene *scene, QGraphicsItem *item);
signals:

public slots:
    void sendTask(STask *task);
    void allocAndSend(QGraphicsScene *scene, QGraphicsItem *item);
};

#endif // STASKPOOL_H
