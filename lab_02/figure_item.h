#ifndef FIGURE_ITEM_H
#define FIGURE_ITEM_H

#include <QGraphicsItemGroup>
#include "figure.h"

typedef QGraphicsItemGroup figure_item_t;

figure_item_t *figure_item_create();

void figure_item_add_line(figure_item_t *item, point_t &p1, point_t &p2);
void figure_item_add_circle(figure_item_t *item, point_t &p1, point_t &p2);

void figure_item_init(figure_item_t *item, figure_t &figure);

void figure_item_move_line(QGraphicsItem *line_item, double dx, double dy);
void figure_item_rotate_line(QGraphicsItem *line_item, double rad, double basex, double basey);
void figure_item_scale_line(QGraphicsItem *line_item, double kx, double ky, double basex, double basey);

void figure_item_move_circle(QGraphicsItem *circle_item, double dx, double dy);
void figure_item_rotate_circle(QGraphicsItem *circle_item, double deg, double basex, double basey);
void figure_item_scale_circle(QGraphicsItem *circle_item, double kx, double ky, double basex, double basey);
void figure_item_move(figure_item_t *item, double dx, double dy);

void figure_item_rotate(figure_item_t *item, double deg, bool base, double basex, double basey);
void figure_item_scale(figure_item_t *item, double kx, double ky, bool base, double basex, double basey);

#endif // FIGURE_ITEM_H
