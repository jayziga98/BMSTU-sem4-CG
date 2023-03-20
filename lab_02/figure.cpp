#include "figure.h"
#include <QDebug>

link_t link_init(int first, int second, LinkType type)
{
    link_t link;

    link.first = first;
    link.second = second;
    link.type = type;

    return link;
}

link_t link_scan(FILE *stream, bool &ok)
{
    int f = 0, s = 0;
    char t;
    ok = (fscanf(stream, "%d%d%c", &f, &s, &t) == 3);

    LinkType type;
    switch (t) {
    case 'l':
        type = LinkType::LINE;
    case 'c':
        type = LinkType::CIRCLE;
    default:
        type = LinkType::LINE;
    }

    return link_init(f, s, type);
}

void figure_add_point(figure_t &figure, point_t &point)
{
    figure.points.push_back(point);
}

void figure_add_link(figure_t &figure, link_t &link)
{
    figure.links.push_back(link);
}

void figure_pop_point(figure_t &figure)
{
    figure.points.pop_back();
}

void figure_pop_link(figure_t &figure)
{
    figure.points.pop_back();
}

figure_t figure_scan(FILE *stream, bool &ok)
{
    figure_t figure;

    int n;

    ok = (fscanf(stream, "%d", &n) == 1);
    for (int i = 0; ok && i < n; i++)
    {
        point_t point = point_scan(stream, ok);
        if (ok)
            figure_add_point(figure, point);
    }

    if (ok)
    {
        int m;
        ok = (fscanf(stream, "%d", &m) == 1);
        for (int i = 0; ok && i < m; i++)
        {
            link_t link = link_scan(stream, ok);
            if (ok)
                figure_add_link(figure, link);
        }
    }

    return figure;
}
