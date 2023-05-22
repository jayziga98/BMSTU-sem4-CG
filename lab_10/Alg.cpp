#include "Alg.h"

#define INVISIBLE 0
#define UPPER_TOP 1
#define LOVER_BOTTOM -1

void yz_rotate(double &y, double &z, double degree, double cy, double cz)
{
    degree *= M_PI / 180.0;
    double dy = y - cy, dz = z - cz;
    y = cy + dy * cos(degree) - dz * sin(degree);
    z = cz + dy * sin(degree) + dz * cos(degree);
}

void xz_rotate(double &x, double &z, double degree, double cx, double cz)
{
    degree *= M_PI / 180.0;
    double dx = x - cx, dz = z - cz;
    x = cx + dx * cos(degree) + dz * sin(degree);
    z = cz - dx * sin(degree) + dz * cos(degree);
}

void xy_rotate(double &x, double &y, double degree, double cx, double cy)
{
    degree *= M_PI / 180.0;
    double dx = x - cx, dy = y - cy;
    x = cx + dx * cos(degree) - dy * sin(degree);
    y = cy + dx * sin(degree) + dy * cos(degree);
}

void move(double &x, double &y, double &z, double dx, double dy, double dz)
{
    x += dx;
    y += dy;
    z += dz;
}

void scale(double &x, double &y, double &z, transform_data &t_data)
{
    x = t_data.scale * x + (1 - t_data.scale) * t_data.center_x;
    y = t_data.scale * y + (1 - t_data.scale) * t_data.center_y;
    z = t_data.scale * z + (1 - t_data.scale) * t_data.center_z;
}

void transform(double x, double y, double z, transform_data t_data, int &res_x, int &res_y)
{
    move(x, y, z, t_data.dx, t_data.dy, t_data.dz);

    yz_rotate(y, z, t_data.cx, t_data.center_y, t_data.center_z);
    xz_rotate(x, z, t_data.cy, t_data.center_x, t_data.center_z);
    xy_rotate(x, y, t_data.cz, t_data.center_x, t_data.center_y);

    scale(x, y, z, t_data);

    res_x = qRound(x);
    res_y = qRound(y);
}

void horizone(int x1, int y1, int x2, int y2, QVector<int> &top, QVector<int> &bottom, QGraphicsScene *scene)
{
    if (x2 == x1)
    {
        top[x2] = qMax(top[x2], y2);
        bottom[x2] = qMin(bottom[x2], y2);
        scene->addLine(x1, y1, x2, y2);
    }
    else
    {
        int tmp_x = x1, tmp_y = y1;
        double m = double(y2 - y1) / double(x2 - x1);
        for (int x = x1; x <= x2; ++x)
        {
            int y = qRound(m * (x - x1) + y1);
            top[x] = qMax(top[x], y);
            bottom[x] = qMin(bottom[x], y);
            scene->addLine(tmp_x, tmp_y, x, y);
        }
    }
}

void
process_edge(int x, int y, int &x_edge, int &y_edge, QVector<int> &top, QVector<int> &bottom, QGraphicsScene *scene)
{
    if (x_edge != -1)
        horizone(x_edge, y_edge, x, y, top, bottom, scene);
    x_edge = x;
    y_edge = y;
}

int visible(int x, int y, QVector<int> &top, QVector<int> &bottom)
{
    int tmp1 = top[x], tmp2 = bottom[x];
    if (y >= top[x])
        return 1;
    if (y <= bottom[x])
        return -1;
    return 0;
}

void intersect(int x1, int y1, int x2, int y2, QVector<int> &horizon, int &xi, int &yi)
{
    int h1 = horizon[x1], h2 = horizon[x2];
    int dx = x2 - x1, dy = y2 - y1, dh = h2 - h1;

    if (dx == 0)
    {
        xi = x2;
        yi = h2;
    }
//    else if (y1 == h1 && y2 == h2)
//    {
//        xi = x1;
//        yi = y1;
//    }
    else
    {
        double m = double(dy) / double(dx);
        xi = x1 - qRound(double(dx * (y1 - h1)) / (dy - dh));
        yi = qRound((xi - x1) * m + y1);
    }
}

void floating_horizon(QGraphicsScene *scene, f_type func, size_data &s_data,transform_data t_data, QSize scene_gometry)
{
    QVector<int> top(scene_gometry.width(), 0), bottom(scene_gometry.width(), scene_gometry.height());

    int x_left = -1, y_left = -1, x_right = -1, y_right = -1;
    for (double z = s_data.ze; z >= s_data.zs; z -= s_data.z_step)
    {
        double y = func(s_data.xs, z);
        int xp, yp;
        transform(s_data.xs, y, z, t_data, xp, yp);

        process_edge(xp, yp, x_left, y_left, top, bottom, scene);
        int p_flag = visible(xp, yp, top, bottom);

        for (double x = s_data.xs; x < s_data.ze; x += s_data.x_step)
        {
            int xc = 0, yc = 0;
            int xi = 0, yi = 0;
            y = func(x, z);
            transform(x, y, z, t_data, xc, yc);

            int t_flag = visible(xc, yc, top, bottom);
            if (t_flag == p_flag)
            {
                if (p_flag != 0)
                    horizone(xp, yp, xc, yc, top, bottom, scene);
            }
            else if (t_flag == 0)
                intersect(xp, yp, xc, yc, p_flag == 1 ? top : bottom, xi, yi);
            else if (t_flag == 1)
            {
                intersect(xp, yp, xc, yc, top, xi, yi);
                horizone(xp, yp, xi, yi, top, bottom, scene);
                if (p_flag != 0)
                {
                    intersect(xp, yp, xc, yc, bottom, xi, yi);
                    horizone(xp, yp, xi, yi, top, bottom, scene);
                }
            }
            else
            {
                intersect(xp, yp, xc, yc, bottom, xi, yi);
                horizone(xp, yp, xi, yi, top, bottom, scene);
                if (p_flag != 0)
                {
                    intersect(xp, yp, xc, yc, top, xi, yi);
                    horizone(xp, yp, xi, yi, top, bottom, scene);
                }
            }
            p_flag = t_flag;
            xp = xc;
            yp = yc;
        }
        process_edge(xp, yp, x_right, y_right, top, bottom, scene);
    }
}
