#ifndef STASK_H
#define STASK_H

#include <QUndoCommand>
#include <vector>
#include "figure_item.h"

class QListWidget;

enum actionType {
    MOVE,
    ROTATE,
    SCALE
};

class STask : public QUndoCommand
{
    int task_id;
    QListWidget *widget;

    int row;
    actionType type;

    figure_item_t *item = nullptr;
    std::vector <double> task_params;

public:
    STask(QListWidget *widget, actionType type, figure_item_t *item, std::vector <double> params, int id);
    void redo();
    void undo();
};

#endif // STASK_H
