#include <math.h>
#include <stdlib.h>

#include "dotprod.h"

double dot_naive(const double* x, const double* y, size_t n)
{
    double sum = 0.0;

    for (size_t i = 0; i < n; i++) {
        sum = sum_naive(sum, mul_naive(x[i], y[i]));
    }

    return sum;
}

double dot_kahan(const double* x, const double* y, size_t n)
{
    double* terms = malloc(n * sizeof(double));
    if (!terms) return 0.0;

    for (size_t i = 0; i < n; i++) {
        terms[i] = mul_naive(x[i], y[i]);
    }

    double sum = sum_kahan(terms, n);

    free(terms);
    return sum;
}

double dot_fma(const double* x, const double* y, size_t n)
{
    double* hi_terms = malloc(n * sizeof(double));
    if (!hi_terms) { return 0.0; }

    double* lo_terms = malloc(n * sizeof(double));
    if (!lo_terms) { free(hi_terms); return 0.0; }

    for (size_t i = 0; i < n; i++) {
        mul_fma(x[i], y[i], &hi_terms[i], &lo_terms[i]);
    }

    double hi_sum = sum_kahan(hi_terms, n);
    double lo_sum = sum_kahan(lo_terms, n);

    double sum = hi_sum + lo_sum;

    free(hi_terms);
    free(lo_terms);

    return sum;
}

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


double mul_naive(const double x, const double y)
{
    return x * y;
}

void mul_fma(const double x, const double y, double* hi, double* lo)
{
    *hi = mul_naive(x, y);          // округленный результат
    *lo = fma(x, y, -(*hi));        // ошибка округления
}