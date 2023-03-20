#include "stask.h"
#include <QListWidget>
#include "figure_item.h"

#define TaskName(name) QString(#name)

STask::STask(QListWidget *widget, actionType type, figure_item_t *item, std::vector <double> params, int id)
{
    this->widget = widget;

    this->widget->setStyleSheet("QListWidget::item[separator=\"true\"] { background: rgb(87, 87, 87); border: 1px solid black; border-radius: 2px;}");
    this->widget->setProperty("separator", true);
    this->widget->setSpacing(1);

    this->type= type;
    this->task_id = id;
    this->task_params = params;
    this->item = item;
    switch (type)
    {
    case MOVE:
        setText(TaskName(Перемещение));
        break;
    case ROTATE:
        setText(TaskName(Поворот));
        break;
    case SCALE:
        setText(TaskName(Масштабирование));
        break;
    default:
        break;
    }
}

void STask::redo()
{
    switch (type)
    {
    case MOVE:
        widget->addItem("Перемещение: dx=" + QString::number(task_params[0], 'g', 3) + " dy=" + QString::number(task_params[1], 'g', 3));
        figure_item_move(item, task_params[0], task_params[1]);
        break;
    case ROTATE:
        widget->addItem("Поворот: угол=" + QString::number(task_params[0], 'g', 3));
        figure_item_rotate(item, task_params[0], task_params[1] != 0.0, task_params[2], task_params[3]);
        break;
    case SCALE:
        widget->addItem("Масштабирование: kx=" + QString::number(task_params[0], 'g', 3) + " ky=" + QString::number(task_params[1], 'g', 3));
        figure_item_scale(item, task_params[0], task_params[1], task_params[2] != 0.0, task_params[3], task_params[4]);
        break;
    default:
        break;
    }
}

void STask::undo()
{
    switch (type)
    {
    case MOVE:
        widget->removeItemWidget(widget->takeItem(widget->count() - 1));
//        widget->takeItem(widget->count() - 1)->setBackground(QBrush(QColor(87, 87, 87)));
        figure_item_move(item, -task_params[0], -task_params[1]);
        break;
    case ROTATE:
        widget->removeItemWidget(widget->takeItem(widget->count() - 1));
        figure_item_rotate(item, -task_params[0], task_params[1], task_params[2], task_params[3]);
        break;
    case SCALE:
        widget->removeItemWidget(widget->takeItem(widget->count() - 1));
        figure_item_scale(item, 1 / task_params[0], 1 / task_params[1], task_params[2], task_params[3], task_params[4]);
        break;
    default:
        break;
    }
}
