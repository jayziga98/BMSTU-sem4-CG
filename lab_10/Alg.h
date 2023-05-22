#ifndef ALG_H
#define ALG_H

#include <QGraphicsScene>
#include <QVector>
#include <QRect>

#include "funcs.h"

struct transform_data
{
    double cx, cy, cz;
    double scale;
    double dx, dy, dz;
    double center_x, center_y, center_z;
};

struct size_data
{
    double xs, xe, x_step;
    double zs, ze, z_step;
};

void floating_horizon(QGraphicsScene *scene, f_type func, size_data &s_data,transform_data t_data, QSize scene_gometry);

#endif
