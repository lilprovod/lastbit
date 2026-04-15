#include "sum.h"

double sum_naive(const double* values, size_t n)
{
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        sum += values[i];
    }

    return sum;
}

double sum_kahan(const double* values, size_t n)
{
    double sum  = 0.0;
    double err = 0.0;

    for (size_t i = 0; i < n; i++) {
        double corrected = values[i] - err;
        double temp = sum + corrected;

        err = (temp - sum) - corrected;
        sum = temp;
    }

    return sum;
}

double sum_ogita_oishi(const double* values, size_t n)
{
    double sum = 0.0;
    double err = 0.0;

    for (size_t i = 0; i < n; i++) {
        double temp  = sum + values[i];
        double delta = (sum - temp) + values[i];

        sum = temp;
        err += delta;
    }

    return sum + err;
}