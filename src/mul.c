#include <math.h>
#include "mul.h"

double mul_naive(const double x, const double y)
{
    return x * y;
}

void two_product_fma(const double x, const double y, double* hi, double* lo)
{
    *hi = mul_naive(x, y);          // округленный результат
    *lo = fma(x, y, -(*hi));        // ошибка округления
}