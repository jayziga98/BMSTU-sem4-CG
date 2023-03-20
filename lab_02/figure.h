#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "point.h"

enum LinkType {
    LINE = 'l',
    CIRCLE = 'c'
};

typedef struct {
    int first;
    int second;
    LinkType type;
} link_t;

typedef struct {
    std::vector <point_t> points;
    std:: vector <link_t> links;
} figure_t;

link_t link_init(int first, int second);
link_t link_scan(FILE *stream, bool &ok);
void figure_add_point(figure_t &figure, point_t &point);
void figure_add_link(figure_t &figure, link_t &link);
void figure_pop_point(figure_t &figure);
figure_t figure_scan(FILE *stream, bool &ok);

#endif // FIGURE_H
