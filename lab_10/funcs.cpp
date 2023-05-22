#include "funcs.h"

double f1(double x, double z)
{
    return x*x + z*z;
}

double f2(double x, double z)
{
    return sin(x) + cos(z);
}

double f3(double x, double z)
{
    return cos(f1(x, z));
}
