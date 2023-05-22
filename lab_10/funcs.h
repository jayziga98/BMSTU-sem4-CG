#ifndef FUNCS_H
#define FUNCS_H

#include <QString>
#include <cmath>

typedef double(*f_type)(double, double);

typedef struct function
{
    QString name;
    f_type funk;
} function;

double f1(double x, double z);
double f2(double x, double z);
double f3(double x, double z);

#endif
