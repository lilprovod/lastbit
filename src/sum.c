#include "sum.h"

double sum_naive(const double x, const double y)
{
    return x + y;
}

double sum_kahan(const double* values, size_t n)
{
    double sum  = 0.0;
    double comp = 0.0;

    for (size_t i = 0; i < n; i++) {
        double corrected = values[i] - comp;
        double temp = sum + corrected;

        comp = (temp - sum) - corrected;
        sum = temp;
    }

    return sum;
}