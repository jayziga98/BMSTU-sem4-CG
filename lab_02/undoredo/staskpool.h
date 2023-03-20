#ifndef STASKPOOL_H
#define STASKPOOL_H

#include <QObject>
#include <QUndoStack>
#include <QAction>
#include "stask.h"

class STask;
class QListWidget;

class STaskPool : public QObject
{
    Q_OBJECT

    QUndoStack stack;

    QListWidget *widget;

    int last_id;

    QAction *undo_;
    QAction *redo_;
public:
    explicit STaskPool(QObject *parent = 0);
    void setWidget(QListWidget *w);
    QAction *getUndoCommand();
    QAction *getRedoCommand();

    STask *allocateTask(actionType type, figure_item_t *item, std::vector <double> params);
signals:

public slots:
    void sendTask(STask *task);
    void allocAndSend(actionType type, figure_item_t *item, std::vector <double> params);
};

#endif // STASKPOOL_H
